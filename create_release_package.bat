cd ..

rmdir FaceCompSource /Q /S
mkdir FaceCompSource

rmdir FaceComp\FaceComp\FaceComp\lib /Q /S
rmdir FaceComp\FaceComp\FaceComp\obj /Q /S
rmdir FaceComp\FaceComp\FaceCompBridge\obj /Q /S
rmdir FaceComp\FaceComp\HttpServer\obj /Q /S
rmdir FaceComp\FaceCompLite\FaceComp\lib /Q /S
rmdir FaceComp\FaceCompLite\FaceComp\obj /Q /S
rmdir FaceComp\FaceCompLite\FaceCompBridge\obj /Q /S
rmdir FaceComp\FaceCompLite\HttpServer\obj /Q /S
rmdir FaceComp\lib\opencv320\3rdparty\compiled_lib\Debug-X64 /Q /S
rmdir FaceComp\lib\opencv320\3rdparty\compiled_lib\Release-X64 /Q /S
del FaceComp\bin\*.pdb /F /Q
del FaceComp\bin\*.ilk /F /Q
del FaceComp\bin\*.exe /F /Q
del FaceComp\bin\*.dll /F /Q
del FaceComp\bin\*.iobj /F /Q
del FaceComp\bin\*.ipdb /F /Q
del FaceComp\bin\*.lib /F /Q
del FaceComp\bin\*.exp /F /Q
del FaceComp\*.db /F /Q
del FaceComp\lib\opencv320\3rdparty\ippicv\ippicv_win\lib\intel64\ippicvmt.lib /F /Q

xcopy FaceComp\* FaceCompSource /E

rmdir FaceCompSource\.svn /Q /S
rmdir FaceCompSource\FaceComp /Q /S
rmdir FaceCompSource\lib\opencv320\.svn /Q /S
rmdir facecompsource\lib\opencv320\apps /q /s
rmdir facecompsource\lib\opencv320\bin /q /s
rmdir facecompsource\lib\opencv320\doc /q /s
rmdir facecompsource\lib\opencv320\platforms /q /s
rmdir facecompsource\lib\opencv320\samples /q /s
rmdir facecompsource\lib\opencv320\modules\calib3d /q /s
rmdir facecompsource\lib\opencv320\modules\core /q /s
rmdir facecompsource\lib\opencv320\modules\cudaarithm /q /s
rmdir facecompsource\lib\opencv320\modules\cudabgsegm /q /s
rmdir facecompsource\lib\opencv320\modules\cudacodec /q /s
rmdir facecompsource\lib\opencv320\modules\cudafeatures2d /q /s
rmdir facecompsource\lib\opencv320\modules\cudafilters /q /s
rmdir facecompsource\lib\opencv320\modules\cudaimgproc /q /s
rmdir facecompsource\lib\opencv320\modules\cudalegacy /q /s
rmdir facecompsource\lib\opencv320\modules\cudaobjdetect /q /s
rmdir facecompsource\lib\opencv320\modules\cudaoptflow /q /s
rmdir facecompsource\lib\opencv320\modules\cudastereo /q /s
rmdir facecompsource\lib\opencv320\modules\cudawarping /q /s
rmdir facecompsource\lib\opencv320\modules\cudev /q /s
rmdir facecompsource\lib\opencv320\modules\features2d /q /s
rmdir facecompsource\lib\opencv320\modules\flann /q /s
rmdir facecompsource\lib\opencv320\modules\highgui /q /s
rmdir facecompsource\lib\opencv320\modules\imgcodecs /q /s
rmdir facecompsource\lib\opencv320\modules\imgproc /q /s
rmdir facecompsource\lib\opencv320\modules\java /q /s
rmdir facecompsource\lib\opencv320\modules\ml /q /s
rmdir facecompsource\lib\opencv320\modules\objdetect /q /s
rmdir facecompsource\lib\opencv320\modules\photo /q /s
rmdir facecompsource\lib\opencv320\modules\python /q /s
rmdir facecompsource\lib\opencv320\modules\python2 /q /s
rmdir facecompsource\lib\opencv320\modules\shape /q /s
rmdir facecompsource\lib\opencv320\modules\stitching /q /s
rmdir facecompsource\lib\opencv320\modules\superres /q /s
rmdir facecompsource\lib\opencv320\modules\ts /q /s
rmdir facecompsource\lib\opencv320\modules\video /q /s
rmdir facecompsource\lib\opencv320\modules\videoio /q /s
rmdir facecompsource\lib\opencv320\modules\videostab /q /s
rmdir facecompsource\lib\opencv320\modules\viz /q /s
rmdir facecompsource\lib\opencv320\modules\world /q /s
rmdir facecompsource\lib\opencv320\contrib\aruco /q /s
rmdir facecompsource\lib\opencv320\contrib\bgsegm /q /s
rmdir facecompsource\lib\opencv320\contrib\bioinspired /q /s
rmdir facecompsource\lib\opencv320\contrib\ccalib /q /s
rmdir facecompsource\lib\opencv320\contrib\cnn_3dobj /q /s
rmdir facecompsource\lib\opencv320\contrib\contrib_world /q /s
rmdir facecompsource\lib\opencv320\contrib\cvv /q /s
rmdir facecompsource\lib\opencv320\contrib\datasets /q /s
rmdir facecompsource\lib\opencv320\contrib\dnn /q /s
rmdir facecompsource\lib\opencv320\contrib\dnns_easily_fooled /q /s
rmdir facecompsource\lib\opencv320\contrib\dpm /q /s
rmdir facecompsource\lib\opencv320\contrib\fuzzy /q /s
rmdir facecompsource\lib\opencv320\contrib\hdf /q /s
rmdir facecompsource\lib\opencv320\contrib\lib /q /s
rmdir facecompsource\lib\opencv320\contrib\line_descriptor /q /s
rmdir facecompsource\lib\opencv320\contrib\matlab /q /s
rmdir facecompsource\lib\opencv320\contrib\modules /q /s
rmdir facecompsource\lib\opencv320\contrib\optflow /q /s
rmdir facecompsource\lib\opencv320\contrib\plot /q /s
del facecompsource\lib\opencv320\contrib\readme.md
rmdir facecompsource\lib\opencv320\contrib\reg /q /s
rmdir facecompsource\lib\opencv320\contrib\rgbd /q /s
rmdir facecompsource\lib\opencv320\contrib\saliency /q /s
rmdir facecompsource\lib\opencv320\contrib\sfm /q /s
rmdir facecompsource\lib\opencv320\contrib\stereo /q /s
rmdir facecompsource\lib\opencv320\contrib\structured_light /q /s
rmdir facecompsource\lib\opencv320\contrib\surface_matching /q /s
rmdir facecompsource\lib\opencv320\contrib\text /q /s
rmdir facecompsource\lib\opencv320\contrib\tracking /q /s
rmdir facecompsource\lib\opencv320\contrib\xfeatures2d /q /s
rmdir facecompsource\lib\opencv320\contrib\ximgproc /q /s
rmdir facecompsource\lib\opencv320\contrib\xobjdetect /q /s
rmdir facecompsource\lib\opencv320\contrib\xphoto /q /s
rmdir facecompsource\lib\opencv320\3rdparty\ffmpeg /q /s
rmdir facecompsource\lib\opencv320\3rdparty\lib\debug-x64 /q /s
rmdir facecompsource\lib\opencv320\3rdparty\lib\release-x64 /q /s

rmdir FaceCompSource\lib\Debug /Q /S
rmdir FaceCompSource\lib\Release /Q /S

del FaceCompSource\FaceComp_VS2015.sln
del FaceCompSource\FaceCompHttpServer_VS2015.sln
del FaceCompSource\create_release_package.bat

"C:\Program Files\7-Zip\7z.exe" a FaceCompSource.zip FaceCompSource\

pause