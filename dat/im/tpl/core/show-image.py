import cv2

# Read image from disk.
node_in_image = in_image
cv2.namedWindow('Image', cv2.WINDOW_NORMAL)
resized_image = cv2.resize(node_in_image, (600, 400))
cv2.imshow('Image', resized_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
