// 
//#include <stdio.h>
#include <assert.h> 
#include <iostream>
#include <string>
#include <fstream>
#include "360SCVPAPI.h"

using namespace std;

extern "C" {
    #include "safestringlib/safe_mem_lib.h"
}

namespace itt4rt {
    param_360SCVP           param;
    unsigned char*          pInputBuffer;
    unsigned char*          pInputBufferlow;
    unsigned char*          pOutputBuffer;
    unsigned char*          pOutputSEI;
    int                     frameWidth;
    int                     frameHeight;
    int                     frameWidthlow;
    int                     frameHeightlow;
    int                     bufferlen;
    int                     bufferlenlow;
    FILE*                   pInputFile;
    FILE*                   pInputFileLow;
	param_oneStream_info*   pTiledBitstreamTotal;


    void SetUp()
    {
      const char *test_1 = "../test/test.265";
      pInputFile = fopen(test_1, "r");
      if (pInputFile == NULL) {
          perror("test.265");
          exit(-1);
      }
      const char *test_2 = "../test/test_low.265";
      pInputFileLow = fopen(test_2, "r");
      if (pInputFileLow == NULL) {
          perror("test_low.265");
          exit(-1);
      }
      //pInputFileLow = fopen("./test_low.265", "rb");
      //if((pInputFile==NULL) ||(pInputFileLow==NULL)){
      //  perror("h265 file");
      //  exit(-1);
        // return;
      //}
      frameWidth = 3840;
      frameHeight = 2048;
      frameWidthlow = 1280;
      frameHeightlow = 768;
      bufferlen = frameWidth * frameHeight * 3 / 2;
      bufferlenlow = frameWidthlow * frameHeightlow * 3 / 2;
      memset_s((void*)&param, sizeof(param_360SCVP), 0);
      cout << "buffer1 size: " << bufferlen << endl; 
      cout << "buffer2 size: " << bufferlenlow << endl; 
      pInputBuffer = new unsigned char[bufferlen];
      pInputBufferlow = new unsigned char[bufferlenlow];
      pOutputBuffer = new unsigned char[bufferlen];
      pOutputSEI = new unsigned char[2000];
      if(!pInputBuffer || !pOutputBuffer || !pInputBufferlow || !pOutputSEI)
        return;
      bufferlen = fread(pInputBuffer, 1, bufferlen, pInputFile);
      
      bufferlenlow = fread(pInputBufferlow, 1, bufferlenlow, pInputFileLow);
      param.pInputBitstream = pInputBuffer;
      param.inputBitstreamLen = bufferlen;
      param.pOutputBitstream = pOutputBuffer;
      param.pInputLowBitstream = pInputBufferlow;
      param.inputLowBistreamLen = bufferlenlow;
      param.frameWidthLow = frameWidthlow;
      param.frameHeightLow = frameHeightlow;
      param.frameWidth = frameWidth;
      param.frameHeight = frameHeight;
      param.pOutputSEI = pOutputSEI;
      param.outputSEILen = 0;

    }

    void TearDown()
    {
      if(pInputBuffer)
        delete pInputBuffer;
      pInputBuffer = NULL;
      if(pInputBufferlow)
        delete pInputBufferlow;
      pInputBufferlow = NULL;
      if(pOutputBuffer)
        delete pOutputBuffer;
      pOutputBuffer = NULL;
      if(pOutputSEI)
        delete pOutputSEI;
      pOutputSEI = NULL;
      fclose(pInputFile);
      fclose(pInputFileLow);
    }

    void Test1(){
      param.usedType = E_STREAM_STITCH_ONLY;
      void* pI360SCVP = I360SCVP_Init(&param);
      bool notnull = (pI360SCVP != NULL);
      //EXPECT_TRUE(pI360SCVP != NULL);
      I360SCVP_unInit(pI360SCVP);
      //EXPECT_TRUE(notnull == true);
      assert(notnull == true);
    }

  void Test2()
  {
      param.usedType = E_MERGE_AND_VIEWPORT;
      param.paramViewPort.faceWidth = frameWidth;// 960; // 7680; //
      param.paramViewPort.faceHeight = frameHeight;// 960;// 3840; //
      param.paramViewPort.geoTypeInput = EGeometryType(E_SVIDEO_EQUIRECT);// SVIDEO_CUBEMAP;// 
      param.paramViewPort.viewportHeight = 960;
      param.paramViewPort.viewportWidth = 960;
      param.paramViewPort.geoTypeOutput = E_SVIDEO_VIEWPORT;
      param.paramViewPort.viewPortYaw = -90;
      param.paramViewPort.viewPortPitch = 0;
      param.paramViewPort.viewPortFOVH = 80;
      param.paramViewPort.viewPortFOVV = 80;
      param.paramViewPort.paramVideoFP.cols = 1;
      param.paramViewPort.paramVideoFP.rows = 1;
      param.paramViewPort.paramVideoFP.faces[0][0].idFace = 0;
      param.paramViewPort.paramVideoFP.faces[0][0].rotFace = NO_TRANSFORM;
      void* pI360SCVP = I360SCVP_Init(&param);
      bool notnull = (pI360SCVP != NULL);
      //EXPECT_TRUE(pI360SCVP != NULL);
      I360SCVP_unInit(pI360SCVP);
      assert(notnull == true);
  }

  void Test3()
  {
      param.usedType = E_PARSER_ONENAL;
      void* pI360SCVP = I360SCVP_Init(&param);
      bool notnull = (pI360SCVP != NULL);
      I360SCVP_unInit(pI360SCVP);
      assert(notnull == true);

      //I360SCVP_unInit(pI360SCVP);
  }

}

int main() {
    cout << "start..." << endl;
    itt4rt::SetUp();
    cout << "tear down" << endl;
    //void* pI360SCVP = 
    itt4rt::Test1();
    cout << "test 1 passed" << endl;
    itt4rt::Test2();
    cout << "test 2 passed" << endl;
    itt4rt::Test3();
    cout << "test 3 passed" << endl;
    //if (pI360SCVP != NULL){
    //  I360SCVP_unInit(pI360SCVP);
    //}
    itt4rt::TearDown();

    return 0;
}