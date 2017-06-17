﻿using GMap.NET;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System;
using System.Drawing;

namespace GoodsTracker
{
    class LayerMap : Object
    {
        static GMapControl  mapControl  = null;
        bool                Visible     = true;
        GMapOverlay         mapOverlay  = null;

        internal LayerMap(GMapControl mc,string name)
        {
            if (mapOverlay == null)
            {
                mapControl = mc;
                mapOverlay = new GMapOverlay(name);
                mapControl.Overlays.Add(mapOverlay);
                Visible = true;
            }
        }

        internal void show()
        {
            if (!Visible)
            {
                Visible = true;
                mapControl.Overlays.Add(mapOverlay);
                mapControl.Refresh();
            }
        }

        internal void hide()
        {
            if (Visible)
            {
                Visible = false;
                mapControl.Overlays.Remove(mapOverlay);
                mapControl.Refresh();
            }
        }

        internal void show(bool flg)
        {
            if (flg) {
                show();
            }
            else
            {
                hide();
            }
        }

        internal bool isVisible()
        {
            return Visible;
        }

        internal void addPosition(PointLatLng position)
        {
            string str = string.Format("Lat.:{0} \n Lng.:{1}", position.Lat, position.Lng);

            addPosition(position, str, GMarkerGoogleType.blue);
        }


        internal void addPosition(PointLatLng position,string str, GMarkerGoogleType color)
        {
            GMarkerGoogle marker;

            marker = new GMarkerGoogle(position, color);
            marker.ToolTipMode = MarkerTooltipMode.Always;
            marker.ToolTipText = string.Format(str);

            mapOverlay.Markers.Add(marker);
        }

        internal void addPosition(PointLatLng position, GMarkerGoogleType color)
        {
            addPosition(position, "", color);
        }

        internal void addPosition(double lat, double lng)
        {
            addPosition(new PointLatLng(lat, lng));
        }

        internal void addFence(string name,Fence fence)
        {
            GMapPolygon     polygon;

            polygon         = new GMapPolygon(fence.Points, name);
            polygon.Fill    = new SolidBrush(Color.FromArgb(50, Color.Red));
            polygon.Stroke  = new Pen(Color.Red, 1);

            mapOverlay.Polygons.Add(polygon);
        }

        internal void removeFenceAt(int index)
        {
            if (index >= 0 && index < mapOverlay.Polygons.Count)
            {
                removeMarkersOfFence(mapOverlay.Polygons[index]);

                mapOverlay.Polygons.RemoveAt(index);
            }
        }

        internal void removeMarkersOfFence(GMapPolygon polygon)
        {
            foreach (PointLatLng point in polygon.Points)
            {
                foreach (GMapMarker marker in mapOverlay.Markers)
                {
                    if (marker.Position.Equals(point))
                    {
                        mapOverlay.Markers.Remove(marker);
                        break;
                    }
                }
            }
        }

        internal void removePositionAt(int index)
        {
            if (index >= 0 && index < mapOverlay.Markers.Count)
            {
                mapOverlay.Markers.RemoveAt(index);
            }
        }

        internal void clearPositions()
        {
            mapOverlay.Markers.Clear();
        }

        internal void addRoute(GMapRoute route)
        {
            mapOverlay.Routes.Add(route);
        }

        internal PointLatLng getPosition(int index)
        {
            return mapOverlay.Markers[index].Position;
        }

        internal PointLatLng getStartPosition()
        {
            return getPosition(0);
        }

        internal PointLatLng getEndPosition()
        {
            return getPosition(mapOverlay.Markers.Count-1);
        }
    }
}
