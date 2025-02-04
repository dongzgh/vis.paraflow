# visProcess

**visProcess** is a powerful and versatile workflow modeler designed to streamline and automate your processes. It is cross-platform, supporting Windows, macOS, and Linux, ensuring that you can use it on any operating system of your choice. 

![Main Window](/res/capture-main-window.png)

## Features

1. **Workflow Modeling**: Easily create and manage complex workflows with an intuitive interface.
2. **Cross-Platform Compatibility**: Use on Windows, macOS, and Linux.
3. **Python Script Generation**: Automatically generate Python scripts directly from your workflows.
4. **Direct Workflow Execution**: Run your workflows directly from the user interface without the need for additional tools.
5. **Extensibility**: Highly open and customizable, allowing users to plug in custom modules effortlessly.

With **visProcess**, you can enhance productivity and efficiency by automating repetitive tasks and integrating custom solutions seamlessly.

## Setup

### macOS

```bash
# Clone the repo.
git clone https://github.com/dongzgh/vis.process.git
cd vis.process

# List all presets (optional)
cmake --list-presets=all
  "(macOS) Debug"
  "(macOS) Release"
  "(Windows) Debug"
  "(Windows) Release"
  "(Linux) Debug"
  "(Linux) Release"

# Cconfigure project.
cmake --preset "(macOS) Debug" # or "(macOS) Release"

# Buil project.
cd build
cd debug # or "release"
cmake --build .

# Run program.
cd bin # vis.process/build/debug/bin or vis.process.process/build/release/bin
./visProcess
```
