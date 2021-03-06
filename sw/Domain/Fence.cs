﻿using System.Data;
using GMap.NET;
using System.Collections.Generic;

namespace GoodsTracker
{
    class Fence
    {
        DataTable           dt;
        List<PointLatLng>   listPoints;
        string              name;

        public DataTable Data { get => dt; set => dt = value; }
        public List<PointLatLng> Points { get => listPoints; set => listPoints = value; }
        public string Name { get => name; set => name = value; }

        internal Fence(string pname)
        {
            name = pname;

            listPoints  = new List<PointLatLng>();

            dt      = new DataTable();

            dt.Columns.Add(new DataColumn("Loc.", typeof(string)));
            dt.Columns.Add(new DataColumn("Latitude", typeof(double)));
            dt.Columns.Add(new DataColumn("Longitude", typeof(double)));
        }

        internal Fence()
        {
            name = "";

            listPoints = new List<PointLatLng>();

            dt = new DataTable();

            dt.Columns.Add(new DataColumn("Loc.", typeof(string)));
            dt.Columns.Add(new DataColumn("Latitude", typeof(double)));
            dt.Columns.Add(new DataColumn("Longitude", typeof(double)));
        }

        internal void insertPositon(double lat, double lng)
        {
            insertPositon(new PointLatLng(lat, lng));
        }

        internal void insertPositon(PointLatLng point)
        {
            dt.Rows.Add(string.Format("{0}", dt.Rows.Count), point.Lat, point.Lng);
            listPoints.Add(point);
        }

        internal void clear()
        {
            dt.Clear();
            listPoints.Clear();
        }

        internal void removePositionAt(int index)
        {
            if (index>=0 && index < dt.Rows.Count)
            {
                dt.Rows.RemoveAt(index);

                listPoints.RemoveAt(index);
            }
        }

        internal bool checkIn(PointLatLng position)
        {
            return true;
        }
    }
}
