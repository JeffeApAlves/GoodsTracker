package com.example.jefferson.goodstracker.Communication;

import android.util.Log;

import com.example.jefferson.goodstracker.Domain.DataTelemetria;
import com.example.jefferson.goodstracker.Domain.LockStatus;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class DecoderFrameOwner extends DecoderFrame{

    class INDEX {

        static public final int ADDRESS = 0;
        static public final int DEST = 1;
        static public final int COUNT = 2;
        static public final int OPERACAO = 3;
        static public final int RESOURCE = 4;
        static public final int SIZE_PAYLOAD = 5;

        static public final int LAT = 6;
        static public final int LNG = 7;

        static public final int ACCEL_X = 8;
        static public final int ACCEL_Y = 9;
        static public final int ACCEL_Z = 10;

        static public final int ROT_X = 11;
        static public final int ROT_Y = 12;
        static public final int ROT_Z = 13;

        static public final int SPEED = 14;
        static public final int LEVEL = 15;
        static public final int TRAVA = 16;
        static public final int TIME = 17;
        static public final int DATE = 18;
    };


    protected DecoderFrameOwner(){

    }

    /**
     *
     * @param data
     * @param ans
     * @return
     */
    @Override
    public boolean str_to_ans(String data, AnsCmd ans){

        DataFrame frame = new DataFrame(FormatFrame.OWNER,TypeFrame.ANS);
        frame.setData(data);

        return frame_to_ans(frame,ans);
    }

    @Override
    public boolean frame_to_chat(DataFrame frame, ChatMessage chat) {

        chat.setPayLoad(frame.getPayLoad());

        return true;
    }

    /**
     *
     * @param frame
     * @param cmd
     * @return
     */
    @Override
    public boolean frame_to_cmd(DataFrame frame,Cmd cmd){

        boolean ret = false;

        try {

            String[] list = frame.getData().split(String.valueOf(CONST_COM.CHAR.SEPARATOR));

            //TODO verificar substring
            // Exclui CheckSum
            frame.setData(frame.getData().substring(0, frame.getData().length() - 2));
            byte cheksumRx = AsHex(list, list.length-1);

            if (frame.checkSum(cheksumRx)) {

                Header header = decoderHeader(list);

                cmd.setHeader(header);

                if (header.getResource().equals(RESOURCE_TYPE.TLM) &&
                        header.getOperation().equals(Operation.AN)) {

                    cmd.setPayload(frame.getPayLoad());
                }

                ret = true;
            } else {

                Log.d("","Erro de CheckSum");
            }
        }
        catch (Exception e) {

            System.out.println("Erro na decodificacao do frame");
            System.out.println(frame.getData());
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return ret;
    }

    /**
     * Answer to Frame
     *
     * @param ans
     * @param frame
     * @return
     */
    @Override
    public boolean ans_to_frame(AnsCmd ans, DataFrame frame){

        Header  header      = ans.getHeader();
        PayLoad payLoad     = buildPayload(ans.getTelemetria());

        frame.setHeader(header);
        frame.setPayLoad(payLoad);

        return true;
    }

    /**
     * Frame to answer
     *
     * @param frame
     * @param ans
     * @return
     */
    @Override
    public boolean frame_to_ans(DataFrame frame, AnsCmd ans) {

        boolean ret     = false;

        try {

            String[] list = frame.getData().split(String.valueOf(CONST_COM.CHAR.SEPARATOR));

            if (list != null && list.length >= 8) {

                //TODO verificar substring
                // Exclui CheckSum
                frame.setData(frame.getData().substring(0, frame.getData().length() - 2));
                byte cheksumRx = AsHex(list, list.length-1);
                ret = frame.calcSum() == cheksumRx;

                if (ret) {

                    Header header = decoderHeader(list);

                    ans.setHeader(header);

                    if (header.getResource().equals(RESOURCE_TYPE.TLM) &&
                        header.getOperation().equals(Operation.AN)) {

                        ans.setTelemetria(decoderTelemetria(list));
                    }

                } else {

                    Log.d("","Erro de CheckSum");
                }
            }
            else {

                Log.d("","Incorreto a quantidade de parametros recebidos");
            }
        }
        catch (Exception e) {

            ret = false;

            System.out.println("Erro na decodificacao do frame");
            System.out.println(frame.getData());
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return ret;
    }

    /**
     * Cmd to Frame
     *
     * @param cmd
     * @param frame
     * @return
     */
    @Override
    public boolean cmd_to_frame(Cmd cmd, DataFrame frame) {

        frame.setPayLoad(cmd.getPayload());
        frame.setHeader(cmd.getHeader());
        return true;
    }

    /**
     * Header to String
     * @param header
     * @return
     */
    @Override
    public String header_to_str(Header header) {

        StringBuilder sb = new StringBuilder();
        boolean ret    = false;

        try {

            // Header
            sb.append(String.format("%05d",header.getAddress()) + CONST_COM.CHAR.SEPARATOR);
            sb.append(String.format("%05d",header.getDest())    + CONST_COM.CHAR.SEPARATOR);
            sb.append(String.format("%05d",header.getCount())   + CONST_COM.CHAR.SEPARATOR);
            sb.append(header.getOperation().name()              + CONST_COM.CHAR.SEPARATOR);
            sb.append(header.getResource());

            ret = true;
        } catch (Exception e) {

            ret = false;

            System.out.println("Erro na codificacao do Header");
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return sb.toString();
    }

    /**
     * String para Header
     * @param data
     * @return
     */
    @Override
    public Header str_to_header(String data) {

        return decoderHeader(data.split(String.valueOf(CONST_COM.CHAR.SEPARATOR)));
    }

    @Override
    public boolean chat_to_frame(ChatMessage chatMessage, DataFrame frame) {

        frame.setPayLoad(chatMessage.getPayLoad());
        frame.setHeader(chatMessage.getHeader());

        return false;
    }

    // Telemetria to Payload
    private static PayLoad buildPayload(DataTelemetria b) {

        PayLoad payload = new PayLoad();
        boolean ret     = false;

        try {

            payload.append(b.getLatitude());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getLongitude());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getAxisX().getValAcceleration());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getAxisY().getValAcceleration());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getAxisZ().getValAcceleration());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getAxisX().getValRotation());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getAxisY().getValRotation());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getAxisZ().getValRotation());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getValSpeed());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getValLevel());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getStatusLock().ordinal());
            payload.append(CONST_COM.CHAR.SEPARATOR);

            //TODO Verificar a formatacao de string de time
            StringBuilder sb;

            sb = new StringBuilder(b.getTime().toString());
            sb = sb.deleteCharAt(2);
            sb = sb.deleteCharAt(4);

            payload.append(sb.toString());
            payload.append(CONST_COM.CHAR.SEPARATOR);

            sb = new StringBuilder(b.getDate().toString());
            sb = sb.deleteCharAt(5);
            sb = sb.deleteCharAt(1);
            payload.append(sb.toString());

            ret = true;

        } catch (Exception e) {

            ret = false;

            System.out.println("Erro na codificacao do frame");
            System.out.println(payload.getData());
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return ret?payload:new PayLoad();
    }

    private static DataTelemetria decoderTelemetria(String[] list) {

        DataTelemetria telemetria = new DataTelemetria();

        try {

            telemetria.setPosition(     AsDouble(list, INDEX.LAT)/100.0,
                                        AsDouble(list, INDEX.LNG)/100.0);

            telemetria.setAcceleration( AsDouble(list, INDEX.ACCEL_X),
                                        AsDouble(list, INDEX.ACCEL_Y),
                                        AsDouble(list, INDEX.ACCEL_Z));

            telemetria.setRotation(     AsDouble(list, INDEX.ROT_X),
                                        AsDouble(list, INDEX.ROT_Y),
                                        AsDouble(list, INDEX.ROT_Z));

            telemetria.setSpeed(        AsDouble(list, INDEX.SPEED));
            telemetria.setLevel(        AsDouble(list, INDEX.LEVEL));
            telemetria.setStatusLock(   AsStatusLoock(list, INDEX.TRAVA));
            telemetria.setDate(         AsDate(list, INDEX.DATE));
            telemetria.setTime (        AsTime(list, INDEX.TIME));

        } catch (Exception e) {

            telemetria = new DataTelemetria();

            System.out.println("Erro na decodificacao dos dados da Telemetria");
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return telemetria;
    }

    private static Header decoderHeader(String[] list) {

        Header header = new Header();

        try {

            header.setAddress(AsInteger(list,       INDEX.ADDRESS));
            header.setDest(AsInteger(list,          INDEX.DEST));
            header.setCount(AsInteger(list,         INDEX.COUNT));
            header.setOperation(AsOperation(list,   INDEX.OPERACAO));
            header.setResource(AsString(list,       INDEX.RESOURCE));
            header.setSizePayLoad(AsInteger(list,   INDEX.SIZE_PAYLOAD));

        } catch (Exception e) {

            header = new Header();

            System.out.println("Erro na decodificacao do Header");
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return header;
    }

    private static Operation AsOperation(String[] list, int oPERACAO) {

        String str = AsString(list, oPERACAO);

        return Operation.valueOf(str );
    }

    private static Date AsTime(String[] list, int index) {

        Date d = new Date();

        if (index < list.length) {

            StringBuilder sb        = new StringBuilder(list[index]);
            SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");

            String str = list[index];

            if (!str.equals("")) {

                sb = sb.insert(2, ":");
                sb = sb.insert(5, ":");
                str = sb.substring(0, str.length() - 4);

                try {
                    d = (Date)format.parse(sb.toString());
                } catch (ParseException e) {
                    e.printStackTrace();
                }
            }
        }

        return d;
    }

    private static Date AsDate(String[] list, int index) {

        Date d = new Date();

        if (index < list.length) {

            StringBuilder sb = new StringBuilder(list[index]);
            SimpleDateFormat format = new SimpleDateFormat("dd/MM/yy");

            String str = list[index];

            if (!str.equals("")) {

                sb = sb.insert(2, "/");
                sb = sb.insert(5, "/");

                try {
                    d = (Date)format.parse(sb.toString());
                } catch (ParseException e) {
                    e.printStackTrace();
                }
            }
        }

        return d;
    }

    private static int AsInteger(String[] list, int index) {

        int dest = 0;

        if (index < list.length) {

            dest = Integer.valueOf(list[index]);
        }

        return dest;
    }

    private static double AsDouble(String[] list, int index) {

        double dest = 0;

        if (index < list.length) {

            String str =list[index].replace(".", ",");

            dest = Double.valueOf(str);
        }

        return dest;
    }

    private static String AsString(String[] list, int index) {

        String dest = "";

        if (index < list.length) {

            dest = list[index];
        }

        return dest;
    }

    private static byte AsHex(String[] list, int index) {

        byte dest = 0;

        if (index < list.length) {

            //TODO Verificar a conversao hex string to byte
            dest = Byte.valueOf(list[index], 16);
        }

        return dest;
    }

    private static LockStatus AsStatusLoock(String[] list, int index) {

        return AsInteger(list, index)!=0 ? LockStatus.UNLOCK:LockStatus.LOCK;
    }
}