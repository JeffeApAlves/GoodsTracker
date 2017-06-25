﻿using System;

namespace GoodsTracker
{
    public enum StatusRx
    {
        RX_FRAME_INIT,
        RX_FRAME_BEGIN,
        RX_FRAME_RX_START,
        RX_FRAME_RX_PAYLOAD,
        RX_FRAME_RX_END,
        RX_FRAME_RX_NL,
        RX_FRAME_RX_CR,
        RX_FRAME_OK,
        RX_FRAME_NOK,
    };

    public enum ResultExec
    {
        EXEC_UNSUCCESS = -3,
        INVALID_CMD = -2,
        INVALID_PARAM = -1,
        EXEC_SUCCESS = 0,
    };

    public class CONST_CHAR
    {        
        public const char RX_FRAME_START    = '[';
        public const char RX_FRAME_END      = ']';
        public const char CR                = '\r';
        public const char LF                = '\n';
        public const char SEPARATOR         = ':';
        public const char NAK               = ((char)0x15);
        public const char ASTERISCO         = '*';
    }

    class Protocol : ThreadRun
    {
        static Protocol singleton = null;

        StatusRx            statusRx        = StatusRx.RX_FRAME_INIT;
        CommunicationFrame  rxFrame;
        CommunicationUnit   currentUnit;

        //Singleton
        public static Protocol Communication
        {
            get
            {
                if (singleton == null)
                {
                    singleton = new Protocol();
                }

                return singleton;
            }
        }

        public override void run()
        {
            currentUnit = CommunicationUnit.getNextUnit();

            if (currentUnit != null)
            {
                processTx();
                processRx();
                currentUnit.processQueues();
            }
        }

        public void processTx()
        {
            if (CommunicationUnit.isAnyCmd())
            {
                CommunicationFrame frame = new CommunicationFrame();
                Cmd cmd = currentUnit.getNextCmd();

                frame.Header    = cmd.Header;
                frame.PayLoad   = cmd.Payload;

                sendFrame(frame);
            }
        }

        void processRx()
        {
            switch (statusRx)
            {
                default:
                case StatusRx.RX_FRAME_INIT:        initRxCMD();        break;
                case StatusRx.RX_FRAME_BEGIN:       rxStartCMD();       break;
                case StatusRx.RX_FRAME_RX_START:    rxPayLoad();        break;
                case StatusRx.RX_FRAME_RX_PAYLOAD:  rxPayLoad();        break;
                case StatusRx.RX_FRAME_RX_END:      rxCR();             break;
                case StatusRx.RX_FRAME_RX_CR:       rxNL();             break;
                case StatusRx.RX_FRAME_RX_NL:       verifyCheckSum();   break;
                case StatusRx.RX_FRAME_OK:          acceptRxFrame();    break;
                case StatusRx.RX_FRAME_NOK:         errorRxFrame();     break;
            }
        }

        void rxStartCMD()
        {
            char ch;

            if (Serial.getRxData(out ch))
            {
                if (ch == CONST_CHAR.RX_FRAME_START)
                {
                    clearRxFrame();

                    setStatusRx(StatusRx.RX_FRAME_RX_START);
                }
            }
        }

        void rxPayLoad()
        {
            char ch;

            if (Serial.getRxData(out ch))
            {
                if (ch == CONST_CHAR.RX_FRAME_START)
                {
                    setStatusRx(StatusRx.RX_FRAME_NOK);
                }
                else if (ch == CONST_CHAR.RX_FRAME_END)
                {
                    setStatusRx(StatusRx.RX_FRAME_RX_END);
                }
                else
                {
                    rxFrame.putByte(ch);

                    setStatusRx(StatusRx.RX_FRAME_RX_PAYLOAD);
                }
            }
        }

        void rxNL()
        {
            char ch;

            if (Serial.getRxData(out ch))
            {
                if (ch == CONST_CHAR.LF)
                {
                    setStatusRx(StatusRx.RX_FRAME_RX_NL);
                }
                else
                {
                    setStatusRx(StatusRx.RX_FRAME_NOK);
                }
            }
        }

        void rxCR()
        {
            char ch;

            if (Serial.getRxData(out ch))
            {
                if (ch == CONST_CHAR.CR)
                {
                    setStatusRx(StatusRx.RX_FRAME_RX_CR);
                }
                else
                {
                    setStatusRx(StatusRx.RX_FRAME_NOK);
                }
            }
        }

        void verifyCheckSum()
        {
            IDecoderFrame decoder = new DecoderFrame();
            AnsCmd ans;

            if (decoder.getValues(out ans, rxFrame))
            {
                CommunicationUnit.addAns(ans);

                setStatusRx(StatusRx.RX_FRAME_OK);
            }
            else
            {
                setStatusRx(StatusRx.RX_FRAME_NOK);
            }
        }

        void acceptRxFrame()
        {
            IDecoderFrame decoder     = new DecoderFrame();
            AnsCmd          ans;

            if (decoder.getValues(out ans, rxFrame))
            {
                CommunicationUnit.addAns(ans);
            }

            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void errorRxFrame()
        {
            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void initRxCMD()
        {
            clearRxFrame();
            setStatusRx(StatusRx.RX_FRAME_BEGIN);
        }

        void setStatusRx(StatusRx sts)
        {
            statusRx = sts;
        }

        void clearRxFrame()
        {
            rxFrame = new CommunicationFrame();
        }

        internal void init()
        {
            setTime(50);

            Serial.Open();
        }

        void sendFrame(CommunicationFrame frame)
        {
            if (frame != null && !frame.isFrameEmpty())
            {
                char[] end = { CONST_CHAR.CR, CONST_CHAR.LF };

                Serial.putTxData(frame.ToCharArray());
                Serial.putTxData(end);
            }
        }

        internal void setFrameRx(CommunicationFrame frame)
        {
            if (frame != null)
            {
                Serial.putRxData(frame.str());
                Serial.putRxData(CONST_CHAR.CR);
                Serial.putRxData(CONST_CHAR.LF);
            }
        }
    }
}
