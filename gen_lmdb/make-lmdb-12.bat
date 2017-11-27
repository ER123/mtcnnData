@echo off

if exist train_lmdb12 rd /q /s train_lmdb12

echo create train_lmdb12...
"C:/Users/salaslyrin/Desktop/mtcnn1120/caffe/convert_imageset.exe" "" C:/Users/salaslyrin/Desktop/gen_pic/12/label-train.txt C:/Users/salaslyrin/Desktop/train_lmdb12 --backend=mtcnn --shuffle=true

echo done.
pause