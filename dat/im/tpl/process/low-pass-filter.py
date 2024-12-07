import cv2

# Apply a low-pass filter to the image.
node_in_image = in_image
node_in_kernel_size = in_kernel_size
node_in_sigma_x = in_sigma_x
node_in_sigma_y = in_sigma_y
node_out_image = cv2.GaussianBlur(node_in_image, (node_in_kernel_size, node_in_kernel_size), node_in_sigma_x, node_in_sigma_y)
