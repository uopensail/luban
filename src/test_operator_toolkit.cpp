#include <iostream>

#include "base64.h"
#include "feature.pb.h"
#include "toolkit.hpp"

int main() {
  std::string data =
      "ChUKDGRfc19uZXdzVHlwZRIFGgMKAQEKaQoNdV9yX2NsaWNrX2ludBJYGlYKVK+/"
      "9wWc+PYFhpP2BYKZ9gW5wvYF55L1BZeI9AXh6PUF8rX1Bdb+9AWf4/"
      "QF6dTyBfWx8gXt9O8F+YzwBbXO7QWizOsFiIDrBfSf5wWzlecFld/"
      "kBQoYCg9kX3NfY29udGVudFR5cGUSBRoDCgEBChMKCWRfcl9jbGljaxIGGgQKAqArChYKDWR"
      "fc19jb3ZlclR5cGUSBRoDCgEDCooCCgx1X3JfZF9zX2NhdDES+"
      "QEK9gEKDWVudGVydGFpbm1lbnQKCWNvbW11bml0eQoJbGlmZXN0eWxlCghwb2xpdGljcwoIc"
      "G9saXRpY3MKCWNvbW11bml0eQoJY29tbXVuaXR5Cghwb2xpdGljcwoJY29tbXVuaXR5Cglsa"
      "WZlc3R5bGUKDWVudGVydGFpbm1lbnQKDWVudGVydGFpbm1lbnQKCHBvbGl0aWNzCgthcnRfY"
      "3VsdHVyZQoJY29tbXVuaXR5Cgljb21tdW5pdHkKBm5hdHVyZQoJY29tbXVuaXR5Cgljb21td"
      "W5pdHkKDWVudGVydGFpbm1lbnQKDWVudGVydGFpbm1lbnQKGQoMZF9yX3JlYWRUaW1lEgkaB"
      "woF8YO15gEK/"
      "AEKDHVfcl9kX3NfY2F0MhLrAQroAQoJY2VsZWJyaXR5CgVtZXRybwoMcmVsYXRpb25zaGlwC"
      "hFwb2xpdGljYWxfc2NhbmRhbAoObG9jYWxfcG9saXRpY3MKBW1ldHJvCgVtZXRybwoObG9jY"
      "WxfcG9saXRpY3MKBW1ldHJvCgxyZWxhdGlvbnNoaXAKCWNlbGVicml0eQoJY2VsZWJyaXR5C"
      "g9nbG9iYWxfcG9saXRpY3MKDGNocmlzdGlhbml0eQoFbWV0cm8KBW1ldHJvCgZhbmltYWwKC"
      "GFjY2lkZW50CgVtZXRybwoJY2VsZWJyaXR5CgljZWxlYnJpdHkKRQoIdV9kX2NhdHMSOQo3C"
      "g1lbnRlcnRhaW5tZW50Cgljb21tdW5pdHkKCHBvbGl0aWNzCglsaWZlc3R5bGUKBmJlYXV0e"
      "QoXCg11Y19tX2ZlZWRGcm9tEgYaBAoC/"
      "wEKOwoNZF9zX2NoYW5uZWxJRBIqCigKAjc4CgI2MwoCOTMKAjUxCgMxMTMKAzEyMwoDMTM2C"
      "gIzOAoDMTAwCkEKEHVfZF92aWRlb1N1YmNhdHMSLQorCgVtZXRybwoFZGFuY2UKBmh1bW91c"
      "goMbW92aWVfc2VyaWVzCgVjcmltZQoeCg51Y19tX2FwcFNvdXJjZRIMCgoKCGxhdW5jaGVyC"
      "hIKCXVjX21fZnJvbRIFGgMKAQEKIwoOZF9zX3NvdXJjZU5hbWUSEQoPCg1UaGUgT2xkIFRpb"
      "WVzCh0KD2Rfc19wdWJsaXNoVGltZRIKGggKBpDSwLnfLgoSCgl1Y19tX3BhZ2USBRoDCgEIC"
      "lYKD3VfZF90ZXh0U3ViY2F0cxJDCkEKCWNlbGVicml0eQoFbWV0cm8KDmxvY2FsX3BvbGl0a"
      "WNzCg9nbG9iYWxfcG9saXRpY3MKDHJlbGF0aW9uc2hpcAoVCgt1X3NfY291bnRyeRIGCgQKA"
      "ktFChYKDWRfcl92aWRlb1RpbWUSBRoDCgEACvoBCgp1X2RfdG9waWNzEusBCugBCg1lbl90b"
      "3BpYzMyXzMxCg1lbl90b3BpYzMyXzI5Cg1lbl90b3BpYzMyXzMwCg1lbl90b3BpYzMyXzE1C"
      "gxlbl90b3BpYzMyXzUKDWVuX3RvcGljMTI4XzcKDmVuX3RvcGljMTI4XzM4Cg1lbl90b3BpY"
      "zEyOF8zCg5lbl90b3BpYzEyOF82OAoOZW5fdG9waWMxMjhfNjcKDmVuX3RvcGljMTI4XzMxC"
      "g5lbl90b3BpYzEyOF80NAoNZW5fdG9waWMxMjhfNgoOZW5fdG9waWMxMjhfMjUKD2VuX3Rvc"
      "GljMTI4XzEwMwrEAQoKdV9yX3NvdXJjZRK1AQqyAQoJQ2VsZWJCdXp6Cg1Tb2NpYWwgU3Vyd"
      "mV5ChBTdHJhbmdlIEJ1dCBUcnVlCgtMaWZlc3R5bGVORwoLIkZha2UiIE5ld3MKDlJlbGlna"
      "W9uIFRydXRoChFWaWRlb0hlYWRsaW5lcyBLRQoQUG9saXRpY3MgTmlnZXJpYQoOVmlkZW8gU"
      "mVwdWJsaWMKEEFyb3VuZCBUaGUgV29ybGQKCVRIRSBTVEFSUwoITkcgU3RhcnMKGwoIZF9zX"
      "2NhdDESDwoNCgthcnRfY3VsdHVyZQoXCghkX3NfY2F0MhILCgkKB2hpc3RvcnkKHAoQdWNfb"
      "V9uZXR3b3JrVHlwZRIICgYKBFdJRkkKWAoPZF9zX2NvdW50cnlDb2RlEkUKQwoCTkcKAktFC"
      "gVPVEhFUgoCR0gKAklOCgJUWgoCVUcKAlpNCgJFVAoCUlcKAk1XCgJHTQoCWlcKAlpBCgJTT"
      "AoCTFIKOgoOdWNfbV9yZXF1ZXN0SWQSKAomCiRhMjIyMWE5OC01MDAwLTQzYTItYWI0YS1hO"
      "WM1NmE5N2RkYjIKGAoOZF9yX2NsaWNrVmFsaWQSBhoECgK+"
      "KwoSCgdkX3JfaW1wEgcaBQoDp4wSClEKCHVfcl9jYXQyEkUKQwoFbWV0cm8KCWNlbGVicml0"
      "eQoObG9jYWxfcG9saXRpY3MKDHJlbGF0aW9uc2hpcAoRcG9saXRpY2FsX3NjYW5kYWwKGQoI"
      "ZF9zX3RhZ3MSDQoLCglvdmVyN2RheXMKFgoMZF9zX2xhbmd1YWdlEgYKBAoCZW4KFwoMZF9y"
      "X2ltcFZhbGlkEgcaBQoD6MsGCuMBCgl1X3JfY2xpY2sS1QEK0gEKCDEyNDQzNTY3CggxMjQz"
      "NDQ2MAoIMTI0MjE1MTAKCDEyNDIyMjc0CggxMjQyNzU3NwoIMTI0MDUwOTUKCDEyMzg3MzUx"
      "CggxMjQxNjA5NwoIMTI0MDk1ODYKCDEyNDAyNTE4CggxMjM5OTAwNwoIMTIzNjQzOTMKCDEy"
      "MzU5OTI1CggxMjMxOTM0MQoIMTIzMjI0MjUKCDEyMjgxNjUzCggxMjI0ODYxMAoIMTIyMzg4"
      "NTYKCDEyMTc3Mzk2CggxMjE3NjA1MQoIMTIxMzYzNDEKEQoFbGFiZWwSCBIGCgQAAIA/"
      "ChIKBmRfc19pZBIIGgYKBJDm9QUKzAIKEnVfcl9kX3Nfc291cmNlTmFtZRK1AgqyAgoJQ2Vs"
      "ZWJCdXp6ChBTdHJhbmdlIEJ1dCBUcnVlCg5WaWRlbyBSZXB1YmxpYwoLIkZha2UiIE5ld3MK"
      "DlJlbGlnaW9uIFRydXRoCghORyBTdGFycwoNU29jaWFsIFN1cnZleQoOUmVsaWdpb24gVHJ1"
      "dGgKEFN0cmFuZ2UgQnV0IFRydWUKC0xpZmVzdHlsZU5HCghORyBTdGFycwoLTGlmZXN0eWxl"
      "TkcKCyJGYWtlIiBOZXdzCg5SZWxpZ2lvbiBUcnV0aAoOVmlkZW8gUmVwdWJsaWMKEVZpZGVv"
      "SGVhZGxpbmVzIEtFChBQb2xpdGljcyBOaWdlcmlhCg5WaWRlbyBSZXB1YmxpYwoQQXJvdW5k"
      "IFRoZSBXb3JsZAoJVEhFIFNUQVJTCghORyBTdGFycwobCg51Y19tX3RpbWVzdGFtcBIJGgcK"
      "BYHhgf4FCh4KBnVfc19pZBIUChIKEGM4NTY2ZmI4M2U3ZTg0YmEKHAoHdWNfbV9pcBIRCg8K"
      "DTIwMy44MS4xNzcuNTQ=";
  auto ddata = base64_decode(data);
  //   std::cout << ddata << std::endl;

  tensorflow::Features* features = new tensorflow::Features();
  features->ParseFromString(ddata);
  // std::cout << features.DebugString() << std::endl;

  Toolkit toolkit("test.toml");

  EntityArray* array = toolkit.process(features);
  for (int i = 0; i < array->size; i++) {
    print_entity(array->array[i]);
  }
  del_entity_array(array);
  delete features;
  return 0;
}