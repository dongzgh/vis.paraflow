from moviepy.audio.io.AudioFileClip import AudioFileClip

# Save video to disk.
node_in_audio: AudioFileClip = in_audio
node_in_path = 'in_path'
node_in_audio.write_audiofile(node_in_path)
