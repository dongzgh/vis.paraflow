from moviepy.video.io.VideoFileClip import VideoFileClip
from moviepy.audio.io.AudioFileClip import AudioFileClip

# Load video file from disk.
n088_in_path = '/root/work/visParaflowAssets/dubbing/res/tutorial.mp4'
n088_out_video = VideoFileClip(n088_in_path)

# Extract audio from video.
n462_in_video: VideoFileClip = n088_out_video
n462_out_extracted_audio: AudioFileClip = n462_in_video.audio
n462_out_extracted_video = n462_in_video.without_audio()

# Save video to disk.
n527_in_audio: AudioFileClip = n462_out_extracted_audio
n527_in_path = '/root/work/visParaflowAssets/dubbing/out/result.wav'
n527_in_audio.write_audiofile(n527_in_path)
