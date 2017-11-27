@echo off

if exist train_lmdb24 rd /q /s train_lmdb24

echo create train_lmdb24...
"C:/Users/salaslyrin/Desktop/mtcnn1120/caffe/convert_imageset.exe" "" C:/Users/salaslyrin/Desktop/gen_pic/24/label-train.txt C:/Users/salaslyrin/Desktop/train_lmdb24 --backend=mtcnn --shuffle=true

echo done.
pause