import cv2

# Read image from disk.
node_in_image = in_image
cv2.namedWindow('Image', cv2.WINDOW_NORMAL)
cv2.resizeWindow('Image', 600, 400)
cv2.imshow('Image', node_in_image)
cv2.waitKey(0)
