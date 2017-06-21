﻿using System;
using System.Windows.Forms;

using GMap.NET;
using GMap.NET.MapProviders;
using System.Collections.Generic;
using GMap.NET.WindowsForms.Markers;
using System.Drawing;

namespace GoodsTracker
{
    public partial class MainForm : Form
    {
        LayerMap    layerFence, layerRoute, layerBehavior;

        STATUS_GUI  statusFence = STATUS_GUI.INIT;
        STATUS_GUI  statusTrip  = STATUS_GUI.INIT;
        ThreadGUI   threadGUI;
        TrackerController trackerController = TrackerController.TrackerCtrl;

        Fence       fence;
        Route       route;

        int itemselected = -1;
        int filterslected = 0;
        int register_count = 0;

        public MainForm()
        {
            InitializeComponent();

            Protocol.Communication.init();

            initAllThreads();

            Serial.Open();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            initMapControl();
            initLayers();
            initPanelTrip();
            initPanelFence();
            initPanelBehavior();
            initPanelConfig();
        }

        private void gMapControl1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            PointLatLng point = gMapControl1.FromLocalToLatLng(e.X, e.Y);

            if (statusTrip.Equals(STATUS_GUI.START_POINT))
            {
                setStartPoint(point);
            }
            else if (statusTrip.Equals(STATUS_GUI.END_POINT))
            {
                setEndPoint(point);
            }
            else if (statusFence.Equals(STATUS_GUI.NEW_FENCE) || statusFence.Equals(STATUS_GUI.ADD_POINTS))
            {
                setFencePoint(point);
            }
        }

        //Seleciona painel trip
        private void button1_Click_1(object sender, EventArgs e)
        {
            selectPanel(panel1);
        }

        //Seleciona painel Fence
        private void button2_Click_1(object sender, EventArgs e)
        {
            selectPanel(panel2);
        }

        //Seleciona painel TelemetriaData
        private void button3_Click_1(object sender, EventArgs e)
        {
            selectPanel(panel3);
        }

        // seleciona painel configuracao
        private void button4_Click_1(object sender, EventArgs e)
        {
            selectPanel(panel4);
        }

        //Inicia nova fence
        private void button5_Click(object sender, EventArgs e)
        {
            if (!statusFence.Equals(STATUS_GUI.NEW_FENCE))
            {
                fence.clear();
                statusFence = STATUS_GUI.NEW_FENCE;
                button5.Enabled = false;
            }
        }

        //Confirma Fence
        private void btn_fence_Click(object sender, EventArgs e)
        {
            if (statusFence.Equals(STATUS_GUI.ADD_POINTS))
            {
                add(fence);
                statusFence = STATUS_GUI.CONFIRM_FENCE;
                button5.Enabled = true;
            }
        }

        // Cancel fence
        private void btn_cancel_Click(object sender, EventArgs e)
        {
            statusFence = STATUS_GUI.INIT_OK;
            button5.Enabled = true;
            fence.clear();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (statusFence.Equals(STATUS_GUI.ADD_POINTS))
            {
                removePositionFence(itemselected);
            }
        }

        // Remove Fence selecionada no combo box
        private void btn_delFence_Click(object sender, EventArgs e)
        {
            removeFenceAt(cbListFence.SelectedIndex);
        }

        // Seleciona ponto no gridview
        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            itemselected = e.RowIndex;

            if (itemselected >= 0)
            {
                txtLat.Text = dataGridView1.Rows[itemselected].Cells[1].Value.ToString();
                txtLng.Text = dataGridView1.Rows[itemselected].Cells[2].Value.ToString();
            }
        }

        //Atualiza zoom do mapa conforme track
        private void tB_Zoom_Scroll(object sender, EventArgs e)
        {
            gMapControl1.Zoom = tB_Zoom.Value;
        }

        //Seleciona layers
        private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (checkedListBox1.GetItemCheckState(3) == CheckState.Checked)
            {
                gMapControl1.MapProvider = GMapProviders.GoogleChinaSatelliteMap;
            }
            else
            {
                gMapControl1.MapProvider = GMapProviders.GoogleMap;
            }

            layerRoute.show(checkedListBox1.GetItemCheckState(0) == CheckState.Checked);
            layerBehavior.show(checkedListBox1.GetItemCheckState(1) == CheckState.Checked);
            layerFence.show(checkedListBox1.GetItemCheckState(2) == CheckState.Checked);
        }

        private void cbFilter_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(filterslected != cbFilter.SelectedIndex)
            {
                filterslected = cbFilter.SelectedIndex;

                updateBehavior();
            }
        }

        private void groupBox1_Click(object sender, System.EventArgs e)
        {
            removeRoute(route);

            initSelectRoute();
        }

        private void groupBox2_Click(object sender, System.EventArgs e)
        {
            statusTrip = STATUS_GUI.END_POINT;
        }

        private void txtLatStart_Enter(object sender, EventArgs e)
        {
            txtLatStart.BackColor = Color.Yellow;
            txtLngStart.BackColor = Color.Yellow;
        }

        private void txtLatStop_Enter(object sender, EventArgs e)
        {
            txtLatStop.BackColor = Color.Yellow;
            txtLngStop.BackColor = Color.Yellow;
        }
        //---------------------------------End Events-----------------------------------

        private void initMapControl()
        {
            gMapControl1.DragButton = MouseButtons.Left;
            gMapControl1.CanDragMap = true;
            gMapControl1.MapProvider = GMapProviders.GoogleMap;
            gMapControl1.MinZoom = 0;
            gMapControl1.MaxZoom = 24;
            gMapControl1.Zoom = 15;
            gMapControl1.AutoScroll = true;
            gMapControl1.Position = new PointLatLng(CAPAO_CITY.LATITUDE, CAPAO_CITY.LONGITUDE);
        }

        private void initPanelTrip()
        {
            route = trackerController.createRoute("Route");
        }

        private void initPanelFence()
        {
            fence = trackerController.createFence();

            dataGridView1.DataSource = fence.Data;

            txtLat.Text = "";
            txtLng.Text = "";
        }

        private void initLayers()
        {
            layerFence = new LayerMap(gMapControl1, "Fence");
            layerRoute = new LayerMap(gMapControl1, "Route");
            layerBehavior = new LayerMap(gMapControl1, "TelemetriaData");
        }

        void initPanelConfig()
        {
            checkedListBox1.SetItemCheckState(0, layerRoute.isVisible() ? CheckState.Checked : CheckState.Unchecked);
            checkedListBox1.SetItemCheckState(1, layerBehavior.isVisible() ? CheckState.Checked : CheckState.Unchecked);
            checkedListBox1.SetItemCheckState(2, layerFence.isVisible() ? CheckState.Checked : CheckState.Unchecked);
            checkedListBox1.SetItemCheckState(3, gMapControl1.MapProvider.Equals(GMapProviders.GoogleChinaSatelliteMap) ? CheckState.Checked : CheckState.Unchecked);
        }

        void initPanelBehavior()
        {
            cbFilter.SelectedIndex  = 0;
            filterslected           = 0;
        }
        //-------------------------------------Fim inits -----------------------------------

        private void initSelectRoute()
        {
            txtLatStop.BackColor    = Color.White;
            txtLngStop.BackColor    = Color.White;
            txtLatStart.BackColor   = Color.White;
            txtLngStart.BackColor   = Color.White;

            route.clear();

            statusTrip              = STATUS_GUI.START_POINT;

            txtLatStart.Text        = "";
            txtLngStart.Text        = "";
            txtLatStop.Text         = "";
            txtLngStop.Text         = "";


            txtLatStart.Focus();
        }

        void updateBehavior()
        {
            BuildTreeView bTV           = new BuildTreeView(tvBehavior);
            List<TelemetriaData> list   = trackerController.getBehaviorFiltered(filterslected);

            bTV.loadlistPointsTreeView(list);
            showMarkerBehavior(list);
        }

        void showMarkerBehavior(List<TelemetriaData> list)
        {
            if (list != null)
            {
                layerBehavior.removeAllMarkers();

                foreach (TelemetriaData b in list)
                {
                    PointLatLng p = new PointLatLng(b.Latitude, b.Longitude);
                    GMarkerGoogleType color = b.OK() ? GMarkerGoogleType.green : GMarkerGoogleType.red;
                    layerBehavior.add(p, b.getStrNOK(), color);
                }
            }
        }

        private void setFencePoint(PointLatLng point)
        {
            txtLat.Text = point.Lat.ToString();
            txtLng.Text = point.Lng.ToString();

            addPositionFence(point);
            statusFence = STATUS_GUI.ADD_POINTS;
            btn_fence.Enabled = true;
        }

        private void setStartPoint(PointLatLng point)
        {
            route.startTrip(point);

            txtLatStart.Text = point.Lat.ToString();
            txtLngStart.Text = point.Lng.ToString();

            txtLatStart.BackColor = Color.FromArgb(61, 120, 165);
            txtLngStart.BackColor = Color.FromArgb(61, 120, 165);

            layerRoute.add(point, GMarkerGoogleType.blue);

            txtLatStop.Focus();

            statusTrip = STATUS_GUI.END_POINT;
        }

        private void setEndPoint(PointLatLng point)
        {
            route.stopTrip(point);

            txtLatStop.Text = point.Lat.ToString();
            txtLngStop.Text = point.Lng.ToString();

            txtLatStop.BackColor = Color.FromArgb(61, 120, 165);
            txtLngStop.BackColor = Color.FromArgb(61, 120, 165);

            layerRoute.add(point, GMarkerGoogleType.blue);

            statusTrip = STATUS_GUI.INIT_OK;

            add(route);
        }

        //seleciona painel
        void selectPanel(Panel p)
        {
            if (p != null)
            {
                bool show = !p.Visible;

                if (show) {

                    panel1.Visible = false;
                    panel2.Visible = false;
                    panel3.Visible = false;
                    panel4.Visible = false;
                }

                p.Visible = show;
            }
        }

        internal void add(Fence fence) {

            string str = string.Format("Fence:{0}", trackerController.Fences.Count+1);

            layerFence.add(str, fence);
            trackerController.addFence(fence);
            cbListFence.Items.Add(str);
            cbListFence.SelectedIndex = cbListFence.Items.Count - 1;
        }

        internal void add(Route r)
        {
            route.createRoute();

            trackerController.addRoute(route);
            layerRoute.add(route);
        }

        void removeFenceAt(int index)
        {
            if (index >= 0)
            {
                layerFence.removeFenceAt(index);
                trackerController.removeFenceAt(index);
                cbListFence.Items.RemoveAt(index);
                cbListFence.Text = "";
            }

            initPanelFence(); //clear entitys
        }

        void removePositionFence(int index)
        {
            if (index >= 0)
            {
                fence.removePositionAt(index);
                layerFence.removeMarkerAt(index);
            }
        }

        void addPositionFence(PointLatLng point)
        {
            fence.insertPositon(point);
            layerFence.add(point, GMarkerGoogleType.yellow);
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            ThreadManager.stop();
        }

        private void panel3_VisibleChanged(object sender, EventArgs e)
        {
            if (panel3.Visible)
            {
                updateBehavior();
            }
        }

        void removeRoute(Route route)
        {
            if (route !=null)
            {
                layerBehavior.removeAllMarkers();
                layerRoute.remove(route);
                trackerController.remove(route);
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            trackerController.requestBehavior();

            if(register_count!= trackerController.getCountRegisters())
            {
                register_count = trackerController.getCountRegisters();
                updateBehavior();
            }
        }

        void initAllThreads()
        {
            threadGUI = new ThreadGUI();
            threadGUI.setTime(1000);
            threadGUI.setUpdate(updateScreen);
            ThreadManager.start();
            timer1.Enabled = true;
        }

        void updateScreen()
        {
            test.AddFrame();
        }
    }

    class CAPAO_CITY
    {
        internal const double LATITUDE  = -23.673326;
        internal const double LONGITUDE = -46.775215;
    }

    enum STATUS_GUI {

        INIT,
        INIT_OK,
        NEW_FENCE,
        ADD_POINTS,
        CONFIRM_FENCE,
        START_POINT,
        END_POINT
    }

    // ############################################  TESTE #######################################################
    class test
    {
        static int indexBehavior = 0;

        static public void AddFrame()
        {

            if (TrackerController.TrackerCtrl.anyRoute() &&
                TrackerController.TrackerCtrl.Routes[0].MapRoute.Points.Count > indexBehavior)
            {
                Random rnd          = new Random();
                TelemetriaData b    = new TelemetriaData();
                PointLatLng p       = TrackerController.TrackerCtrl.Routes[0].MapRoute.Points[indexBehavior++];

                b = new TelemetriaData();
                b.DateTime = DateTime.Now;
                b.setPosition(p.Lat, p.Lng);
                b.setAcceleration(rnd.Next(0, 4), rnd.Next(5, 9), rnd.Next(10, 14));
                b.setSpeed(rnd.Next(40,120));
                b.setLevel(rnd.Next(900,1000));

                DecoderFrame decoder        = new DecoderFrame();
                CommunicationFrame frame    = new CommunicationFrame();
                Cmd cmd                     = new Cmd(RESOURCE.BEHAVIOR);
//                Tracker t                   = new Tracker(2);
                cmd.Operation               = Operation.AN;
                cmd.Dest                    = 1;

                decoder.setHeader(ref frame, TrackerController.TrackerCtrl.Tracker , cmd);
                decoder.setPayLoad(ref frame,   b);
               
                Protocol.Communication.setFrameRx(  CONST_CHAR.RX_FRAME_START + 
                                                    frame.Frame +
                                                    CONST_CHAR.SEPARATOR +
//                                                    CONST_CHAR.ASTERISCO + 
                                                    frame.checkSum().ToString() +
                                                    CONST_CHAR.RX_FRAME_END +
                                                    CONST_CHAR.CR + 
                                                    CONST_CHAR.LF);
            }
        }
    }
}