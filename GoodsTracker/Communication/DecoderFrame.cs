﻿using System;
using System.Diagnostics;

namespace GoodsTracker
{
    internal class DecoderFrame : IDecoderFrame
    {
        enum DATA_INDEX
        {
            ADDRESS = 0,
            DEST = 1,
            COUNT = 2,
            OPERACAO = 3,
            RESOURCE = 4,
            SIZE_PAYLOAD = 5,

            LAT =6,
            LNG = 7,

            ACCEL_X = 8,
            ACCEL_Y = 9,
            ACCEL_Z = 10,

            ROT_X = 11,
            ROT_Y = 12,
            ROT_Z = 13,

            SPEED = 14,
            LEVEL = 15,
            TRAVA = 16,
            DATETIME = 17,

            CHECKSUM = 18,
        }

        static public bool setHeader(Header header)
        {
            bool ret    = false;

            try
            {
                // Header
                header.Append(header.Address.ToString("D5")+ CONST_CHAR.SEPARATOR);
                header.Append(header.Dest.ToString("D5")+ CONST_CHAR.SEPARATOR);
                header.Append(header.Count.ToString("D5") + CONST_CHAR.SEPARATOR);
                header.Append(header.Operation.ToString()+ CONST_CHAR.SEPARATOR);
                header.Append(header.Resource);

                ret = true;
            }
            catch (Exception e)
            {
                ret = false;

                Console.WriteLine("Erro na codificacao do Header");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return ret;
        }

        public bool setValues(out PayLoad payload, TelemetriaData b)
        {
            bool ret    = false;
            payload     = new PayLoad();

            try
            {
                payload.Append(b.Latitude);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.Longitude);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisX.Acceleration.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisY.Acceleration.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisZ.Acceleration.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisX.Rotation.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisY.Rotation.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisZ.Rotation.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.Speed.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.Level.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.StatusLock.ToString());
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.DateTime.ToString().Replace(CONST_CHAR.SEPARATOR, '.'));

                ret = true;
            }
            catch (Exception e)
            {
                ret = false;

                Console.WriteLine("Erro na codificacao do frame");
                Console.WriteLine(payload.Data);
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return ret;
        }

        public bool getValues(out AnsCmd ans, DataFrame frame)
        {
            bool ret    = false;
            ans         = new AnsCmd();

            try
            {
                string[] list = frame.Data.Split(CONST_CHAR.SEPARATOR);

                if (list != null && list.Length >= 8)
                {
                    // Exclui CheckSum
                    frame.Data = frame.Data.Substring(0, frame.Data.Length - 2);
                    byte cheksumRx = AsHex(list, DATA_INDEX.CHECKSUM);
                    ret = frame.checkSum() == cheksumRx;

                    if (ret)
                    {
                        ans.Header      = decoderHeader(list);

                        if (ans.Header.Resource.Equals(RESOURCE.TELEMETRIA) && 
                            ans.Header.Operation.Equals(Operation.AN))
                        {
                            ans.Telemetria = decoderTelemetria(list);
                        }
                    }
                    else
                    {
                        Console.WriteLine("Erro de CheckSum");
                    }
                }
                else
                {
                    Console.WriteLine("Incorreto a quantidade de parametros recebidos");
                }
            }
            catch (Exception e)
            {
                ret = false;

                Console.WriteLine("Erro na decodificacao do frame");
                Console.WriteLine(frame.Data);
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return ret;
        }

        private TelemetriaData decoderTelemetria(string[] list)
        {
            TelemetriaData telemetria = new TelemetriaData();

            try
            {
                telemetria.setPosition(AsDouble(list, DATA_INDEX.LAT),
                                        AsDouble(list, DATA_INDEX.LNG));

                telemetria.setAcceleration(AsDouble(list, DATA_INDEX.ACCEL_X),
                                            AsDouble(list, DATA_INDEX.ACCEL_Y),
                                            AsDouble(list, DATA_INDEX.ACCEL_Z));

                telemetria.setRotation(AsDouble(list, DATA_INDEX.ROT_X),
                                        AsDouble(list, DATA_INDEX.ROT_Y),
                                        AsDouble(list, DATA_INDEX.ROT_Z));

                telemetria.Speed.Val = AsDouble(list, DATA_INDEX.SPEED);
                telemetria.Level.Val = AsDouble(list, DATA_INDEX.LEVEL);
                telemetria.StatusLock = AsBool(list, DATA_INDEX.TRAVA);
                telemetria.DateTime = AsDateTime(list, DATA_INDEX.DATETIME);
            }
            catch (Exception e)
            {
                telemetria = new TelemetriaData();

                Console.WriteLine("Erro na decodificacao dos dados da Telemetria");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return telemetria;
        }
    
        private Header decoderHeader(string[] list)
        {
            Header header = new Header();

            try
            {
                header.Address = AsInteger(list, DATA_INDEX.ADDRESS);
                header.Dest = AsInteger(list, DATA_INDEX.DEST);
                header.Count = AsInteger(list, DATA_INDEX.COUNT);
                header.Operation = AsOperation(list, DATA_INDEX.OPERACAO);
                header.Resource = AsString(list, DATA_INDEX.RESOURCE);
                header.SizePayLoad = AsInteger(list, DATA_INDEX.SIZE_PAYLOAD);
            }
            catch (Exception e)
            {
                header = new Header();

                Console.WriteLine("Erro na codificacao do Header");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return header;
        }

        static private Operation AsOperation(string[] list, DATA_INDEX oPERACAO)
        {
            string str = AsString(list, oPERACAO);

            return (Operation)Enum.Parse(typeof(Operation), str);
        }

        static private DateTime AsDateTime(string[] list, DATA_INDEX index)
        {
            DateTime d = Convert.ToDateTime("01/01/1900");

            if ((int)index < list.Length)
            {
                d = Convert.ToDateTime(list[(int)index].Replace('.', CONST_CHAR.SEPARATOR));
            }
            return d;
        }

        static private int AsInteger(string[] list, DATA_INDEX index)
        {
            int dest = 0;

            if ((int)index < list.Length)
            {
                dest = Convert.ToInt16(list[(int)index]);
            }

            return dest;
        }

        static private double AsDouble(string[] list, DATA_INDEX index)
        {
            double dest = 0;

            if ((int)index < list.Length)
            {
                    dest = Convert.ToDouble(list[(int)index].Replace(".", ","));
            }

            return dest;
        }

        static private string AsString(string[] list, DATA_INDEX index)
        {
            string dest = "";

            if ((int)index < list.Length)
            {
                dest = list[(int)index];
            }

            return dest;
        }

        static private byte AsHex(string[] list, DATA_INDEX index)
        {
            byte dest = 0;

            if ((int)index < list.Length)
            {
                dest = (byte)Convert.ToInt16(list[(int)index],16);
            }

            return dest;
        }

        static private bool AsBool(string[] list, DATA_INDEX index)
        {
            int dest = 0;

            if ((int)index < list.Length)
            {
                dest = (int)Convert.ToInt16(list[(int)index]);
            }

            return dest>0 ? true:false;
        }
    }
}