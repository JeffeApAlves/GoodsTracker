#include "XF1.h"
#include "Serialization.h"


void tlm2String(DataTLM* info,ArrayPayLoad* ans){

	if(info!=NULL && ans!= NULL){

		clearArrayPayLoad(ans);

		XF1_xsprintf(ans->Data,"%.8f:%.8f:%d:%d:%d:%d:%d:%d:%d:%d:%d:%s:%s",
				info->Lat,
				info->Lng,
				info->Axis[AXIS_X],
				info->Axis[AXIS_Y],
				info->Axis[AXIS_Z],
				info->G[AXIS_X],
				info->G[AXIS_Y],
				info->G[AXIS_Z],
				info->Speed,
				info->Level,
				info->Lock,
				info->Time,
				info->Date);

		ans->Count = strlen(ans->Data);
	}
}
//------------------------------------------------------------------------

