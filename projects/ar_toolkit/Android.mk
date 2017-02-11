# Copyright 2014 Google Inc. All Rights Reserved.
# Distributed under the Project Tango Preview Development Kit (PDK) Agreement.
# CONFIDENTIAL. AUTHORIZED USE ONLY. DO NOT REDISTRIBUTE.
# NDK import module for resolving symbols from selected private Android APIs.
### IMPORTANT ###
# This module has a non-standard extra step. Usage:
#   $(call import-module,AndroidSystemLibs)
#   $(call module-add-static-depends,<MY_MODULE>,AndroidSystemLibs)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE            := ar_toolkit
LOCAL_CFLAGS            := -O3
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_LDLIBS            := -llog
LOCAL_SRC_FILES         := src/AR/arDetectMarker.c \
                           src/AR/ar3DCreateHandle.c \
                           src/AR/ar3DUtil.c \
                           src/AR/arCreateHandle.c \
                           src/AR/arDetectMarker2.c \
                           src/AR/arFilterTransMat.c \
                           src/AR/arGetLine.c \
                           src/AR/arGetMarkerInfo.c \
                           src/AR/arGetTransMat.c \
                           src/AR/arGetTransMatStereo.c \
                           src/AR/arImageProc.c \
                           src/AR/arLabeling.c \
                           src/AR/arPattAttach.c \
                           src/AR/arPattCreateHandle.c \
                           src/AR/arPattGetID.c \
                           src/AR/arPattLoad.c \
                           src/AR/arPattSave.c \
                           src/AR/arUtil.c \
                           src/AR/mAlloc.c \
                           src/AR/mAllocDup.c \
                           src/AR/mAllocInv.c \
                           src/AR/mAllocMul.c \
                           src/AR/mAllocTrans.c \
                           src/AR/mAllocUnit.c \
                           src/AR/mDet.c \
                           src/AR/mDisp.c \
                           src/AR/mDup.c \
                           src/AR/mFree.c \
                           src/AR/mInv.c \
                           src/AR/mMul.c \
                           src/AR/mPCA.c \
                           src/AR/mSelfInv.c \
                           src/AR/mTrans.c \
                           src/AR/mUnit.c \
                           src/AR/paramChangeSize.c \
                           src/AR/paramClear.c \
                           src/AR/paramDecomp.c \
                           src/AR/paramDisp.c \
                           src/AR/paramDistortion.c \
                           src/AR/paramFile.c \
                           src/AR/paramGetPerspective.c \
                           src/AR/paramLT.c \
                           src/AR/vAlloc.c \
                           src/AR/vDisp.c \
                           src/AR/vFree.c \
                           src/AR/vHouse.c \
                           src/AR/vInnerP.c \
                           src/AR/vTridiag.c \
                           src/ARICP/icpCalibStereo.c \
                           src/ARICP/icpCore.c \
                           src/ARICP/icpHandle.c \
                           src/ARICP/icpPoint.c \
                           src/ARICP/icpPointRobust.c \
                           src/ARICP/icpStereoHandle.c \
                           src/ARICP/icpStereoPoint.c \
                           src/ARICP/icpStereoPointRobust.c \
                           src/ARICP/icpUtil.c \
                           src/AR/arLabelingSub/arLabelingSubDBI3C565.c \
                           src/AR/arLabelingSub/arLabelingSubDBI3CA4444.c \
                           src/AR/arLabelingSub/arLabelingSubDBI3CA5551.c \
                           src/AR/arLabelingSub/arLabelingSubDBI3CA.c \
                           src/AR/arLabelingSub/arLabelingSubDBI3C.c \
                           src/AR/arLabelingSub/arLabelingSubDBIA3C.c \
                           src/AR/arLabelingSub/arLabelingSubDBIC.c \
                           src/AR/arLabelingSub/arLabelingSubDBICY.c \
                           src/AR/arLabelingSub/arLabelingSubDBIYC.c \
                           src/AR/arLabelingSub/arLabelingSubDBR3C565.c \
                           src/AR/arLabelingSub/arLabelingSubDBR3CA4444.c \
                           src/AR/arLabelingSub/arLabelingSubDBR3CA5551.c \
                           src/AR/arLabelingSub/arLabelingSubDBR3CA.c \
                           src/AR/arLabelingSub/arLabelingSubDBR3C.c \
                           src/AR/arLabelingSub/arLabelingSubDBRA3C.c \
                           src/AR/arLabelingSub/arLabelingSubDBRC.c \
                           src/AR/arLabelingSub/arLabelingSubDBRCY.c \
                           src/AR/arLabelingSub/arLabelingSubDBRYC.c \
                           src/AR/arLabelingSub/arLabelingSubDBZ.c \
                           src/AR/arLabelingSub/arLabelingSubDWI3C565.c \
                           src/AR/arLabelingSub/arLabelingSubDWI3CA4444.c \
                           src/AR/arLabelingSub/arLabelingSubDWI3CA5551.c \
                           src/AR/arLabelingSub/arLabelingSubDWI3CA.c \
                           src/AR/arLabelingSub/arLabelingSubDWI3C.c \
                           src/AR/arLabelingSub/arLabelingSubDWIA3C.c \
                           src/AR/arLabelingSub/arLabelingSubDWIC.c \
                           src/AR/arLabelingSub/arLabelingSubDWICY.c \
                           src/AR/arLabelingSub/arLabelingSubDWIYC.c \
                           src/AR/arLabelingSub/arLabelingSubDWR3C565.c \
                           src/AR/arLabelingSub/arLabelingSubDWR3CA4444.c \
                           src/AR/arLabelingSub/arLabelingSubDWR3CA5551.c \
                           src/AR/arLabelingSub/arLabelingSubDWR3CA.c \
                           src/AR/arLabelingSub/arLabelingSubDWR3C.c \
                           src/AR/arLabelingSub/arLabelingSubDWRA3C.c \
                           src/AR/arLabelingSub/arLabelingSubDWRC.c \
                           src/AR/arLabelingSub/arLabelingSubDWRCY.c \
                           src/AR/arLabelingSub/arLabelingSubDWRYC.c \
                           src/AR/arLabelingSub/arLabelingSubDWZ.c \
                           src/AR/arLabelingSub/arLabelingSubEBI3C565.c \
                           src/AR/arLabelingSub/arLabelingSubEBI3CA4444.c \
                           src/AR/arLabelingSub/arLabelingSubEBI3CA5551.c \
                           src/AR/arLabelingSub/arLabelingSubEBI3CA.c \
                           src/AR/arLabelingSub/arLabelingSubEBI3C.c \
                           src/AR/arLabelingSub/arLabelingSubEBIA3C.c \
                           src/AR/arLabelingSub/arLabelingSubEBIC.c \
                           src/AR/arLabelingSub/arLabelingSubEBICY.c \
                           src/AR/arLabelingSub/arLabelingSubEBIYC.c \
                           src/AR/arLabelingSub/arLabelingSubEBR3C565.c \
                           src/AR/arLabelingSub/arLabelingSubEBR3CA4444.c \
                           src/AR/arLabelingSub/arLabelingSubEBR3CA5551.c \
                           src/AR/arLabelingSub/arLabelingSubEBR3CA.c \
                           src/AR/arLabelingSub/arLabelingSubEBR3C.c \
                           src/AR/arLabelingSub/arLabelingSubEBRA3C.c \
                           src/AR/arLabelingSub/arLabelingSubEBRC.c \
                           src/AR/arLabelingSub/arLabelingSubEBRCY.c \
                           src/AR/arLabelingSub/arLabelingSubEBRYC.c \
                           src/AR/arLabelingSub/arLabelingSubEBZ.c \
                           src/AR/arLabelingSub/arLabelingSubEWI3C565.c \
                           src/AR/arLabelingSub/arLabelingSubEWI3CA4444.c \
                           src/AR/arLabelingSub/arLabelingSubEWI3CA5551.c \
                           src/AR/arLabelingSub/arLabelingSubEWI3CA.c \
                           src/AR/arLabelingSub/arLabelingSubEWI3C.c \
                           src/AR/arLabelingSub/arLabelingSubEWIA3C.c \
                           src/AR/arLabelingSub/arLabelingSubEWIC.c \
                           src/AR/arLabelingSub/arLabelingSubEWICY.c \
                           src/AR/arLabelingSub/arLabelingSubEWIYC.c \
                           src/AR/arLabelingSub/arLabelingSubEWR3C565.c \
                           src/AR/arLabelingSub/arLabelingSubEWR3CA4444.c \
                           src/AR/arLabelingSub/arLabelingSubEWR3CA5551.c \
                           src/AR/arLabelingSub/arLabelingSubEWR3CA.c \
                           src/AR/arLabelingSub/arLabelingSubEWR3C.c \
                           src/AR/arLabelingSub/arLabelingSubEWRA3C.c \
                           src/AR/arLabelingSub/arLabelingSubEWRC.c \
                           src/AR/arLabelingSub/arLabelingSubEWRCY.c \
                           src/AR/arLabelingSub/arLabelingSubEWRYC.c \
                           src/AR/arLabelingSub/arLabelingSubEWZ.c \
                           src/ARMulti/arMultiFreeConfig.c \
                           src/ARMulti/arMultiGetTransMat.c \
                           src/ARMulti/arMultiGetTransMatStereo.c \
                           src/ARMulti/arMultiReadConfigFile.c

include $(BUILD_SHARED_LIBRARY)
