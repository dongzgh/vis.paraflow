from moviepy.video.io.VideoFileClip import VideoFileClip
from moviepy.audio.io.AudioFileClip import AudioFileClip

# Load video file from disk.
n088_in_path = '/Users/dong/Documents/github/visparaflow/assets/video/res/tutorial.mp4'
n088_out_video = VideoFileClip(n088_in_path)

# Extract audio from video.
n462_in_video: VideoFileClip = n088_out_video
n462_out_extracted_audio: AudioFileClip = n462_in_video.audio
n462_out_extracted_video = n462_in_video.without_audio()

# Save video to disk.
n542_in_video: VideoFileClip = n462_out_extracted_video
n542_in_path = '/Users/dong/Documents/github/visparaflow/assets/video/out/result.mp4'
n542_in_video.write_videofile(n542_in_path)