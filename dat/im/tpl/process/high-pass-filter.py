import cv2
import numpy as np

# Apply a high-pass filter to the image.
node_in_image = in_image
node_in_kernel = in_kernel
kernel = np.array([[-1, -1, -1], [-1, node_in_kernel, -1], [-1, -1, -1]])
node_out_image = cv2.filter2D(node_in_image, -1, kernel)
