﻿namespace GoodsTracker
{
    internal delegate ResultExec CallBackAnsCmd(AnsCmd dados);

    internal class AnsCmd
    {
        string nameCmd;

        ObjectValueRX dadosRx;

        public string NameCmd { get => nameCmd; set => nameCmd = value; }
        public ObjectValueRX DadosRx { get => dadosRx; set => dadosRx = value; }
    }
}