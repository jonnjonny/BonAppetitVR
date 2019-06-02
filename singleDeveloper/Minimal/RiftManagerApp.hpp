#include "Core.h"


class RiftManagerApp {
protected:
  ovrSession _session;


  ovrHmdDesc _hmdDesc;


  ovrGraphicsLuid _luid;


public:
  RiftManagerApp() {
    if( !OVR_SUCCESS( ovr_Create( &_session, &_luid ) ) ) {
      FAIL( "Unable to create HMD session" );
    }

    _hmdDesc = ovr_GetHmdDesc( _session );
  }


  ~RiftManagerApp() {
    ovr_Destroy( _session );
    _session = nullptr;
  }
};
