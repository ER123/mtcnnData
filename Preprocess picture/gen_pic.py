import numpy as np 
import sys, getopt
import cv2
import os
import numpy.random as npr
from utils import IoU
import argparse

def proces_pic(size, pic_path, anno_path):

    stdsize = size

    hasPts = True
    if size == 12:
        hasPts=False

    root_path = pic_path                   #gen data path 
    anno_file = anno_path             #raw label file
    #im_dir = "D:/facedetect-like/CelebA/Img/img_celeba.7z/train/"   #raw data path

    pos_save_dir = root_path + "/positive"
    part_save_dir = root_path +  "/part"
    neg_save_dir = root_path + "/negative"

    def mkr(dr):
        if not os.path.exists(dr):
            os.mkdir(dr)

    mkr(root_path)
    mkr(pos_save_dir)
    mkr(part_save_dir)
    mkr(neg_save_dir)

    f1 = open(os.path.join(root_path, 'pos_' + str(stdsize) + '.txt'), 'w')
    f2 = open(os.path.join(root_path, 'neg_' + str(stdsize) + '.txt'), 'w')
    f3 = open(os.path.join(root_path, 'part_' + str(stdsize) + '.txt'), 'w')
    with open(anno_file, 'r') as f:
        annotations = f.readlines()
    num = len(annotations)
    print "%d pics in total" % num
    p_idx = 0 # positive
    n_idx = 0 # negative
    d_idx = 0 # dont care
    idx = 0
    box_idx = 0

    with_points = 14
    without_point = 4
    rect_point_num = 4
    key_point_num = 10


    for annotation in annotations:
        annotation = annotation.strip().split(' ')
        im_path = annotation[0]
        #bbox = map(float, annotation[1:5])
        annotation_1 = map(float, annotation[1:])
        boxes = np.array(annotation_1, dtype=np.float32).reshape(-1,with_points)
        #print boxes
        bbox = np.zeros((boxes.shape[0], rect_point_num)) #n*4 dim
        pts = np.zeros((boxes.shape[0], key_point_num)) #n*10 dim 

        for boxes_num in xrange(boxes.shape[0]):
            s_bbox = map(float, annotation[1 + boxes_num * with_points: 5 + boxes_num * with_points])# single box has points 
            bbox[boxes_num:] = np.array(s_bbox, dtype=np.float32).reshape(-1,rect_point_num)

            s_pts = map(float, annotation[5 + boxes_num * with_points: 15 + boxes_num * with_points])
            pts[boxes_num:] = np.array(s_pts, dtype=np.float32).reshape(-1,key_point_num)

        #multi faces in single pic with points
        #boxes = np.array(bbox, dtype=np.float32).reshape(-1, 14)

        img = cv2.imread(im_path)
        idx += 1
        if idx % 100 == 0:
            print idx, "images done"

        height, width, channel = img.shape

        idx_of_pts = 0
        backupPts = pts[:]        
        for box in boxes:

            neg_num = 0
            while neg_num < 30:
                baseNum = 40
                while baseNum >= min(width, height) / 2:
                    baseNum -= 5
                size = npr.randint(baseNum, min(width, height) / 2)
                #size = npr.randint(40, min(width, height) / 2)
                nx = npr.randint(0, width - size)
                ny = npr.randint(0, height - size)
                crop_box = np.array([nx, ny, nx + size, ny + size])

                Iou = IoU(crop_box, bbox)

                cropped_im = img[ny : ny + size, nx : nx + size, :]
                resized_im = cv2.resize(cropped_im, (stdsize, stdsize), interpolation=cv2.INTER_LINEAR)

                if np.max(Iou) < 0.3:
                    # Iou with all gts must below 0.3
                    save_file = os.path.join(neg_save_dir, "%s.jpg"%n_idx)
                    f2.write(neg_save_dir + "/%s"%n_idx + ' 0\n')
                    cv2.imwrite(save_file, resized_im)
                    n_idx += 1
                    neg_num += 1

            # box (x_left, y_top, x_right, y_bottom)
            x1, y1, x2, y2 = box[0:4]
            w = x2 - x1 + 1
            h = y2 - y1 + 1

            # ignore small faces
            # in case the ground truth boxes of small faces are not accurate
            if max(w, h) < 12 or x1 < 0 or y1 < 0:
                continue

            # generate positive examples and part faces
            # print "pts: ", backupPts[idx_of_pts]
            for i in range(20):
                pts0 =  backupPts[idx_of_pts]
                size = npr.randint(int(min(w, h) * 0.8), np.ceil(1.25 * max(w, h)))

                # delta here is the offset of box center
                delta_x = npr.randint(-w * 0.2, w * 0.2)
                delta_y = npr.randint(-h * 0.2, h * 0.2)

                nx1 = max(x1 + w / 2 + delta_x - size / 2, 0)
                ny1 = max(y1 + h / 2 + delta_y - size / 2, 0)
                nx2 = nx1 + size
                ny2 = ny1 + size

                if nx2 > width or ny2 > height:
                    continue
                crop_box = np.array([nx1, ny1, nx2, ny2])

                offset_x1 = (x1 - nx1) / float(size)
                offset_y1 = (y1 - ny1) / float(size)
                offset_x2 = (x2 - nx1) / float(size)
                offset_y2 = (y2 - ny1) / float(size)

                if hasPts == True:
                    pts_res = np.zeros(len(pts0))
                    for k in range(len(pts0) / 2):
                        pts_res[k*2] = (pts0[k*2] - nx1) / float(size);
                        pts_res[k*2+1] = (pts0[k*2+1] - ny1) / float(size);

                cropped_im = img[int(ny1) : int(ny2), int(nx1) : int(nx2), :]
                resized_im = cv2.resize(cropped_im, (stdsize, stdsize), interpolation=cv2.INTER_LINEAR)

                box_ = box.reshape(1, -1)
                if IoU(crop_box, box_) >= 0.65:
                    save_file = os.path.join(pos_save_dir, "%s.jpg"%p_idx)
                    f1.write(pos_save_dir + "/%s"%p_idx + ' 1 %f %f %f %f'%(offset_x1, offset_y1, offset_x2, offset_y2))

                    if hasPts == True:
                        for k in range(len(pts_res)):
                            f1.write(" %f" % pts_res[k])
                    f1.write("\n")
                    cv2.imwrite(save_file, resized_im)
                    p_idx += 1
                elif IoU(crop_box, box_) >= 0.4:
                    save_file = os.path.join(part_save_dir, "%s.jpg"%d_idx)
                    f3.write(part_save_dir + "/%s"%d_idx + ' -1 %f %f %f %f'%(offset_x1, offset_y1, offset_x2, offset_y2))

                    if hasPts == True:
                        for k in range(len(pts_res)):
                            f3.write(" %f" % pts_res[k])
                    f3.write("\n")
                    cv2.imwrite(save_file, resized_im)
                    d_idx += 1

            idx_of_pts += 1
            box_idx += 1
            print "%s images %s part done, pos: %s part: %s neg: %s"%(idx, idx_of_pts, p_idx, d_idx, n_idx)

    f1.close()
    f2.close()
    f3.close()

def parse_args():
    parser = argparse.ArgumentParser(description='Gen positive negative part pictures.')
    parser.add_argument('--net', dest='net', help='P R O net',type=int)
    parser.add_argument('--gen_path', dest='gen_path', help='dataset floder', type=str)
    parser.add_argument('--label_path', dest='label_path', help='label path', type=str)

    args = parser.parse_args()
    return args

if __name__ == '__main__':
    args = parse_args()
    proces_pic(args.net, args.gen_path, args.label_path)