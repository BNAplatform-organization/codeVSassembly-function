rem .\CUCorMat.exe D:\BNU_data_S12 0.2 N s 0.2 0.15 0.1 0.05
rem .\CUCorMat.exe D:\BNU_data_S2 0.2 N s 0.2 0.15 0.1 0.05 
rem .\CUCorMat.exe D:\BNU_data_S11 0.2 N s 0.2 0.15 0.1 0.05

rem .\Degree.exe D:\BNU_data_S12
rem.\Cp.exe D:\BNU_data_S12 0
rem.\Cp.exe D:\BNU_data_S2 0
.\BFS_MulCPU.exe D:\BNU_data_S12 0
.\BFS_MulCPU.exe D:\BNU_data_S11 0
.\BFS_MulCPU.exe D:\BNU_data_S2 0

rem .\ConvertNII.exe E:\xumo\4Dnii\4Dnii_197_58523_0.151%%_2.modu D:\dataTumor1\mask.nii 0.2
@pause
