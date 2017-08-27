#include "XF1.h"
#include "Serialization.h"


void tlm2String(DataTLM* info,ArrayPayLoad* ans){

	if(info!=NULL && ans!= NULL){

		clearArrayPayLoad(ans);

	XF1_xsprintf(ans->Data,"%.8f:%.8f:%d:%d:%d:%d:%d:%d:%d:%d:%d:%s:%s",
				info->GPS.Lat,
				info->GPS.Lng,
				info->Accelerometer.Axis[AXIS_X],
				info->Accelerometer.Axis[AXIS_Y],
				info->Accelerometer.Axis[AXIS_Z],
				info->Accelerometer.G[AXIS_X],
				info->Accelerometer.G[AXIS_Y],
				info->Accelerometer.G[AXIS_Z],
				info->GPS.Speed,
				info->Tank.Level,
				info->Tank.Lock,
				info->GPS.Time,
				info->GPS.Date);

		ans->Length = strlen(ans->Data);
	}
}
//------------------------------------------------------------------------

