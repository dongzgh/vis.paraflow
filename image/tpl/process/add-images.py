import cv2

# Apply a low-pass filter to the image.
node_in_image_1 = in_image_1
node_in_image_2 = in_image_2
node_in_image_1_weight = in_image_1_weight
node_in_image_2_weight = in_image_2_weight
node_in_gamma = in_gamma
node_out_image = cv2.addWeighted(node_in_image_1, node_in_image_1_weight, node_in_image_2, node_in_image_2_weight, node_in_gamma)
