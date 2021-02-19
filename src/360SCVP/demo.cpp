// 
//#include <stdio.h>
#include <assert.h> 
#include <iostream>
#include <string>
#include <fstream>
#include "360SCVPAPI.h"

using namespace std;

// extern "C" {
//     #include "safestringlib/safe_mem_lib.h"
// }
#include <string.h>

#define memset_s(a, s, b) memset(a, b, s)
//#define memcpy_s(a, s, b) memcpy(a, b, s)
#define memcpy_s(a, s1, b, s2) memcpy(a, b, s1)
#define memmove_s(a, s1, b, s2) memmove(a, b, s1)


namespace itt4rt {

    #define Default_VewPortFOV 80

    param_360SCVP           param;
    uint8_t*                pInputBuffer;  // uint8_t
    uint8_t*                pInputBufferlow;
    uint8_t*                pOutputBuffer;
    uint8_t*                pOutputSEI;
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
      //const char *test_1 = "../test/360_pattern2.h265";
      pInputFile = fopen(test_1, "r");
      if (pInputFile == NULL) {
          perror("test.265");
          exit(-1);
      }
      const char *test_2 = "../test/test_low.265";
      //const char *test_2 = "../test/360_pattern2_small.h265";
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
      //frameHeight = 1920;
      //frameWidthlow = 960;
      //frameHeightlow = 480;
      frameWidthlow = 1280;
      frameHeightlow = 768;
      bufferlen = frameWidth * frameHeight * 5;
      bufferlenlow = frameWidthlow * frameHeightlow * 5;
      memset_s((void*)&param, sizeof(param_360SCVP), 0);
      cout << "buffer1 size: " << bufferlen << endl; 
      cout << "buffer2 size: " << bufferlenlow << endl; 
      pInputBuffer = new uint8_t[bufferlen];
      pInputBufferlow = new uint8_t[bufferlenlow];
      pOutputBuffer = new uint8_t[bufferlen];
      pOutputSEI = new uint8_t[2000];
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
        delete [] pInputBuffer;
      pInputBuffer = NULL;
      if(pInputBufferlow)
        delete [] pInputBufferlow;
      pInputBufferlow = NULL;
      if(pOutputBuffer)
        delete [] pOutputBuffer;
      pOutputBuffer = NULL;
      if(pOutputSEI)
        delete [] pOutputSEI;
      pOutputSEI = NULL;
      fclose(pInputFile);
      fclose(pInputFileLow);
    }

    // void Test1(){
    //   param.usedType = E_STREAM_STITCH_ONLY;
    //   void* pI360SCVP = I360SCVP_Init(&param);
    //   bool notnull = (pI360SCVP != NULL);
    //   //EXPECT_TRUE(pI360SCVP != NULL);
    //   I360SCVP_unInit(pI360SCVP);
    //   //EXPECT_TRUE(notnull == true);
    //   assert(notnull == true);
    // }

/*
  void test_tile_numbers()
  {
      int32_t tileNum_fast, tileNum_legacy;
      TileDef pOutTile[1024];
      Param_ViewportOutput paramViewportOutput;

      param.paramViewPort.faceWidth = frameWidth;// 960; // 7680; //
      param.paramViewPort.faceHeight = frameHeight;// 960;// 3840; //
      param.paramViewPort.geoTypeInput = EGeometryType(E_SVIDEO_EQUIRECT);// SVIDEO_CUBEMAP;// 
      param.paramViewPort.viewportHeight = 960;
      param.paramViewPort.viewportWidth = 960;
      param.paramViewPort.geoTypeOutput = E_SVIDEO_VIEWPORT;
      param.paramViewPort.tileNumCol = 10;
      param.paramViewPort.tileNumRow = 8;
      param.paramViewPort.viewPortYaw = 90;
      param.paramViewPort.viewPortPitch = 0;
      param.paramViewPort.viewPortFOVH = 80;
      param.paramViewPort.viewPortFOVV = 80;
      param.usedType = E_VIEWPORT_ONLY;
      param.paramViewPort.paramVideoFP.cols = 1;
      param.paramViewPort.paramVideoFP.rows = 1;
      param.paramViewPort.paramVideoFP.faces[0][0].faceWidth = param.paramViewPort.faceWidth;
      param.paramViewPort.paramVideoFP.faces[0][0].faceHeight = param.paramViewPort.faceHeight;
      param.paramViewPort.paramVideoFP.faces[0][0].idFace = 1;
      param.paramViewPort.paramVideoFP.faces[0][0].rotFace = NO_TRANSFORM;

      void* pI360SCVP = I360SCVP_Init(&param);
      assert(pI360SCVP != NULL);
      if (!pI360SCVP)
      {
          I360SCVP_unInit(pI360SCVP);
          printf( "Init 360SCVP failure: pI360SCVP is NULL!!!\n");
          return;
      }
      float yaw =  param.paramViewPort.viewPortYaw ;
      float pitch = param.paramViewPort.viewPortPitch;
      cout << "yaw: " << yaw << ", pitch:" << pitch << endl;
      tileNum_fast = I360SCVP_getTilesInViewport(pOutTile, &paramViewportOutput, pI360SCVP);
      cout << "tileNum_fast:" << tileNum_fast << endl;
      I360SCVP_process(&param, pI360SCVP);
      I360SCVP_SetParameter(pI360SCVP, ID_SCVP_PARAM_VIEWPORT, &param.paramViewPort);
      tileNum_legacy = I360SCVP_GetTilesByLegacyWay(&pOutTile[tileNum_fast], pI360SCVP);
      cout << "tileNum_legacy:" << tileNum_legacy << endl;
      I360SCVP_unInit(pI360SCVP);
      assert(tileNum_fast >= 0);
      assert(tileNum_legacy >= 0);
  }
*/
  void Test2()
  {
      int ret = 0;

      param.usedType = E_MERGE_AND_VIEWPORT;
      param.paramViewPort.faceWidth = param.frameWidth;// 960; // 7680; //
      param.paramViewPort.faceHeight = param.frameHeight;// 960;// 3840; //

      param.paramViewPort.geoTypeInput = EGeometryType(E_SVIDEO_EQUIRECT);// SVIDEO_CUBEMAP;// 
      param.paramViewPort.geoTypeOutput = E_SVIDEO_VIEWPORT;
      // m_viewport_w
      param.paramViewPort.viewportHeight = 960;
      param.paramViewPort.viewportWidth = 960;
      param.paramViewPort.viewPortYaw = 0;  // m_yaw
      param.paramViewPort.viewPortPitch = 0;
      param.paramViewPort.viewPortFOVH = Default_VewPortFOV;
      param.paramViewPort.viewPortFOVV = Default_VewPortFOV;
      /*
      param.paramViewPort.paramVideoFP.cols = 1;
      param.paramViewPort.paramVideoFP.rows = 1;
      param.paramViewPort.paramVideoFP.faces[0][0].idFace = 0;
      param.paramViewPort.paramVideoFP.faces[0][0].rotFace = NO_TRANSFORM;
      */
      void* pI360SCVP = I360SCVP_Init(&param);
      if (!pI360SCVP)
      {
          I360SCVP_unInit(pI360SCVP);
          cout << "I360SCVP_Init failed" << endl;
          return;
      }

      cout << "Original merged stream length:" << param.inputBitstreamLen << endl;
      ret = I360SCVP_setViewPort(pI360SCVP, param.paramViewPort.viewPortYaw, param.paramViewPort.viewPortPitch);
      assert(ret==0);
      ret = I360SCVP_process(&param, pI360SCVP);
      cout << "result:" << ret << endl;
      assert(ret==0);
      assert(param.outputBitstreamLen > 0);
      cout << "Actual merged stream length:" << param.outputBitstreamLen << endl;
      cout << "SEI NAL length:" << param.outputSEILen << endl;

      // now we persist param.pOutputBitstream
      //std::fstream file = std::fstream("merged.h265", std::ios::out | std::ios::binary);
      FILE* fp = fopen("merged.h265", "wb");
      fwrite(param.pOutputBitstream, 1, param.outputBitstreamLen, fp);
      fwrite(param.pOutputSEI, 1, param.outputSEILen, fp);
      fflush(fp);
      fclose(fp);
      //bool notnull = (pI360SCVP != NULL);
      //EXPECT_TRUE(pI360SCVP != NULL);
      I360SCVP_unInit(pI360SCVP);
      //assert(notnull == true);
  }

  // void Test3()
  // {
  //     param.usedType = E_PARSER_ONENAL;
  //     void* pI360SCVP = I360SCVP_Init(&param);
  //     bool notnull = (pI360SCVP != NULL);
  //     I360SCVP_unInit(pI360SCVP);
  //     assert(notnull == true);

  //     //I360SCVP_unInit(pI360SCVP);
  // }

}

int main() {
    cout << "start..." << endl;
    itt4rt::SetUp();
    //void* pI360SCVP = 
    //itt4rt::Test1();
    //cout << "test 1 passed" << endl;
    itt4rt::Test2();
    cout << "test 2 passed" << endl;
    //itt4rt::Test3();
    //cout << "test 3 passed" << endl;
    //if (pI360SCVP != NULL){
    //  I360SCVP_unInit(pI360SCVP);
    //}
    //itt4rt::test_tile_numbers();
    itt4rt::TearDown();
    cout << "tear down" << endl;
    return 0;
}