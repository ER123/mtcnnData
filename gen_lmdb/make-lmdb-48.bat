@echo off

if exist train_lmdb48 rd /q /s train_lmdb48

echo create train_lmdb48...
"C:/Users/salaslyrin/Desktop/mtcnn1120/caffe/convert_imageset.exe" "" C:/Users/salaslyrin/Desktop/gen_pic/48/label-train.txt C:/Users/salaslyrin/Desktop/train_lmdb48 --backend=mtcnn --shuffle=true

echo done.
pause