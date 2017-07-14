package com.example.jefferson.goodstracker.Communication;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import com.example.jefferson.goodstracker.RabbitMQ.AMQPCommunication;

/**
 * Created by Jefferson on 08/0`/2017.
 */

abstract public class Communication extends Object implements Communic,ObservableCommunication,Runnable  {

    private static Communication        instance            = null;
    private static TYPE_COMMUNICATION   type                = TYPE_COMMUNICATION.NONE;
    private static Map<String,Cmd>      txCmds              = new HashMap<String, Cmd>();
    private static Map<String,Cmd>      cmds                = new HashMap<String, Cmd>();
    private static List<AnsCmd>         answers             = new ArrayList<AnsCmd>();
    private static Map<Integer,ObserverCommunication>units  = new HashMap<Integer,ObserverCommunication>();

    private static WorkerProducer       workerProducer      = null;
    private static WorkerConsumer       workerConsumer      = null;

    protected Communication(){

    }

    /**
     * Metodo construtor do tipo de comunicacao
     *
     * @param type_com
     */
    public static boolean create(TYPE_COMMUNICATION type_com) {

        boolean success = false;

        try {

            if (type != type_com) {

                if(instance!=null){

                    instance.close();
                }

                type = type_com;

                if (!TYPE_COMMUNICATION.NONE.equals(type_com)) {

                    switch (type) {

                        case AMQP:      instance    = new AMQPCommunication();  break;
                        case SERIAL:    instance    = null;                     break; // Comunicacao via serial
                    }

                    if(instance!=null){

                        instance.startCommunication();

                        success = true;
                    }
                }
            }

        }catch (Exception e){

            success = false;
            Log.d("","Problemas na inicializacao da conexao");
        }

        return success;
    }

    /**
     * Inicia a thead que fara a conexao
     *
     */
    protected void startCommunication() {

        new Thread(this).start();
    }

    /**
     *
     * A conexao sera chamada em uma Thread
     *
     */
    @Override
    public void run() {

        try{

            if(connect()){

                startWorkerProducer();
                startWorkerConsumer();
                notifyConnEstablished();
            }

        }catch (Exception e){

            Log.d("","Problemas na inicialicalizaco da conexao");
        }
    }

    @Override
    public void close(){

        if(workerProducer !=null) {
            workerProducer.interrupt();
            workerProducer.interrupt();
        }

        if(workerConsumer!=null) {
            workerConsumer.interrupt();
        }
    }

    public static boolean isAnyTxCmd() {

        return txCmds.size() > 0;
    }


    public static boolean isAnyAns() {

        return answers.size() > 0;
    }

    public static boolean isAnyCmd() {

        return cmds.size() > 0;
    }

    public static void addAns(AnsCmd ans) {

        if (ans != null) {

            answers.add(ans);
        }
    }

    public static void removeAns(AnsCmd ans) {

        if (ans != null) {

            answers.remove(ans);
        }
    }

    public static void removeTxCmd(Cmd cmd) {

        if (cmd != null) {

            txCmds.remove(cmd.getResource());
        }
    }

    public static void addTxCmd(Cmd cmd) {

        if (cmd != null) {

            txCmds.put(cmd.getResource(),cmd);
        }
    }

    public static void removeCmd(Cmd cmd) {

        if (cmd != null) {

            cmds.remove(cmd.getResource());
        }
    }

    public static void addCmd(Cmd cmd) {

        if (cmd != null) {

            cmds.put(cmd.getResource(),cmd);
        }
    }

    public static Cmd findCmdByAnswer(AnsCmd ans) {

        Header ans_header = ans.getHeader();

        if(txCmds.containsKey(ans_header.getResource())) {

            Cmd cmd = txCmds.get(ans_header.getResource());

            Header cmd_header = cmd.getHeader();

            if ((   ans_header.getResource()    == cmd_header.getResource())    &&
                (   ans_header.getDest()        == cmd.getAddress())            &&
                (   ans_header.getCount()       == cmd_header.getCount())       &&
                    ans_header.getAddress()     == cmd.getDest()) {

                return cmd;
            }
        }

        return null;
    }

    public static CommunicationUnit[] getArrayOfUnit() {

        CommunicationUnit[] ret = new CommunicationUnit[units.size()];

        if (units.size() > 0) {

            Log.d("","Teste");

            // TODO Verificar a conversao de map para array
            ret = units.values().toArray(ret);
        }

        return ret;
    }

    public static Cmd[] getArrayOfCmd() {

        Cmd[] ret = new Cmd[cmds.size()];

        if (isAnyCmd()) {

            //TODO Verificar a conversao de map para array
            ret = cmds.values().toArray(ret);
        }

        return ret;
    }

    public static AnsCmd[] getArrayOfAns() {

        AnsCmd[] ret = new AnsCmd[answers.size()];

        if (isAnyAns()) {

            ret = answers.toArray(ret);
        }

        return ret;
    }

    public void acceptAnswer(AnsCmd ans) {

        if (ans != null) {

            addAns(ans);

            notifyObserver(ans);

            Cmd cmd = findCmdByAnswer(ans);

            removeTxCmd(cmd);
            removeAns(ans);
        }
    }

    public static Cmd takeFirstCmd() {

        return isAnyCmd()?getArrayOfCmd()[0]:null;
    }

    public static AnsCmd takeFirstAns() {

        return isAnyAns()?getArrayOfAns()[0]:null;
    }

    @Override
    public void registerObserver(ObserverCommunication observer) throws IOException {

        if (observer != null) {

            units.put(observer.getAddress(),observer);
        }
    }

    @Override
    public void removeObserver(ObserverCommunication observer){

        units.remove(observer.getAddress());
    }

    @Override
    public void notifyObserver(Object obj){

        if(obj instanceof AnsCmd) {

            AnsCmd ans = (AnsCmd)obj;

            if (units.containsKey(ans.getHeader().getAddress())) {

                ObserverCommunication ob = units.get(ans.getHeader().getAddress());

                // Notifica apenas entidade respectiva do address
                ob.updateAnswer(ans);
            }
        }
    }


    /**
     *
     * Notifica conexao estabelecida
     *
     */
    @Override
    public void notifyConnEstablished(){

        for(ObserverCommunication ob : units.values()){

            ob.connEstablished();
        }
    }

    /**
     *
     * Retorna o singleton
     *
     * @return
     */
    public static Communication getInstance() {
        return instance;
    }



    /**
     *
     * Thread para processamento da pilha de saida
     */
    @Override
    public void startWorkerProducer() {

        workerProducer = new WorkerProducer();
        workerProducer.start();
    }

    /**
     * Thread para processamento da pilha de entrada
     *
     */
    @Override
    public void startWorkerConsumer() {

        workerConsumer = new WorkerConsumer();
        workerConsumer.start();
    }

    /**
     * Envia um comando para a fila de mensagens da Thread de transmissao
     *
     * @param cmd
     */
    public void publish(Cmd cmd) {

        addCmd(cmd);
        workerProducer.flush(IdMessage.CMD);
    }

    /**
     *
     * @param ans
     */
    public void publish(AnsCmd ans) {

        // TODO Criar lista de resposta para ser usada como slave
        workerProducer.flush(IdMessage.ANS);
    }

    /**
     * Processa o frame recebido
     *
     * @param frame
     */
    @Override
    public void consumerFrame(DataFrame frame){

        AnsCmd ans = new AnsCmd();

        IDecoder decoder = DecoderFrame.create(frame);

        if(decoder.frame_to_ans(frame,ans)){

            acceptAnswer(ans);
        }
    }

    public void consumerCmd(DataFrame frame){

        Cmd cmd = new Cmd();

        IDecoder decoder = DecoderFrame.create(frame);

        if(decoder.frame_to_cmd(frame,cmd)){

            //TODO acceptCmd(cmd)
        }
    }

    public Handler getHandlerConsumer(){

        return workerConsumer.getHandler();
    }

    /**
     *
     * Hook para processamento das mensagens
     *
     */
    private class WorkerConsumer extends Thread implements Handler.Callback {

        public Handler handler=null;

        @Override
        public void run() {

            Looper.prepare();
            handler = new Handler(this);
            Looper.loop();
        }

        @Override
        public boolean handleMessage(Message msg) {

            String data = msg.getData().getString("PAYLOAD");

            doWork(IdMessage.values()[msg.what],data);

            return true;
        }

        public void doWork(IdMessage i,String data) {

            switch (i){

                case ANS:   consumerFrame(  new DataFrame(data,TypeFrame.OWNER));   break;
                case CMD:   consumerCmd(    new DataFrame(data,TypeFrame.OWNER));   break;
            }
        }

        public Handler getHandler() {
            return handler;
        }
    }

    private class WorkerProducer extends Thread implements Handler.Callback {

        public Handler handler=null;

        @Override
        public void run() {

            Looper.prepare();
            handler = new Handler(this);
            Looper.loop();
        }

        @Override
        public boolean  handleMessage(Message msg) {

            doWork(IdMessage.values()[msg.what]);

            return true;
        }

        public void doWork(IdMessage i) {

            boolean flg = false;

            DataFrame   frame   = new DataFrame(TypeFrame.OWNER);
            IDecoder    decoder = DecoderFrame.create(frame);

            switch (i){

                case ANS:   flg = decoder.ans_to_frame(takeFirstAns(),frame);break;
                case CMD:   flg = decoder.cmd_to_frame(takeFirstCmd(),frame);break;
            }

            if(flg){

                producerFrame(frame);
            }
        }

        public Handler getHandler() {
            return handler;
        }

        /**
         * O conteudo foi previamente inserido nos containers respectivo
         */
        public void flush(IdMessage id) {

            if (handler!=null) {
                handler.sendEmptyMessage(id.ordinal());
            }
        }
    }
}