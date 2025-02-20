# visParaflow

**visParaflow** is a powerful and versatile workflow modeler designed to streamline and automate your processes. It is cross-platform, supporting Windows, macOS, and Linux, ensuring that you can use it on any operating system of your choice.

![Main Window](/res/capture-main-window.png)

## Features

1. **Workflow Modeling**: Easily create and manage complex workflows with an intuitive interface.
2. **Cross-Platform Compatibility**: Use on Windows, macOS, and Linux.
3. **Python Script Generation**: Automatically generate Python scripts directly from your workflows.
4. **Direct Workflow Execution**: Run your workflows directly from the user interface without the need for additional tools.
5. **Extensibility**: Highly open and customizable, allowing users to plug in custom modules effortlessly.

With **visParaflow**, you can enhance productivity and efficiency by automating repetitive tasks and integrating custom solutions seamlessly.

## Setup

```shell
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
cmake --preset "(macOS) Debug" # based on the platform and configuration

# Buil the project.
cd build
cd debug # or "release"
cmake --build .

# Run the program.
cd bin # vis.process/build/debug/bin or vis.process.process/build/release/bin
./visParaflow # on windows: visParaflow
```

## Deployment

### Windows

- Locate `windeployqt` or `windeployqt6` in `<path to Qt toolset>\bin`:

```shell
windeployqt <path-to-your-executable> # for Qt5
windeployqt6 <path-to-your-executable> # for Qt6
```

- Open `make-installer-win32.nsi` in Visual Studio Code
- `F1` to open `Command Palette` and select `Tasks: Run Task`
- Select `Build NSIS Script` from the drop down list
- Installer will be created in the `scripts` folder

## Prepare Data

### Folder Structure

The `dat` folder should contain the following structure to organize your workflow project data:

```text
dat/
├── my_workflow/
│   ├── ico/
│   │   ├── section1
|   |   |   ├── icon1.ico
|   |   |   ├── ...
│   │   ├── secion2
|   |   |   ├── icon1.ico
|   |   |   ├── ...
│   ├── tpl/
│   │   ├── section1
|   |   |   ├── tpl1.py
|   |   |   ├── ...
│   │   ├── secion2
|   |   |   ├── tpl1.py
|   |   |   ├── ...
│   ├── res/
│   │   ├── ...
│   ├── wf/
│   │   ├── ...
|   ├──section1.json
|   ├──section2.json
|   ├──...
|   ├──main.json
|   ├──requirements.txt
```

- `my_workflow/`: This is your custom workflow project folder.
  - `ico/`: Place your icon files here, organized by sections.
  - `tpl/`: Place your Python template files here, organized by sections.
  - `res/`: Place your resource or asset files here.
  - `wf/`: Place your workflow files here.
- `section[#n].json`: section description file
- `main.json`: main description file
- `requirements.txt`: Python package requirements file.

### Writing Data

#### Main JSON File Format

The description files in this project are structured in JSON format to provide a clear and organized way to define various sections and main configurations. These files include details about the project, such as the product name, version, and included components. Each section is described in its respective JSON file, ensuring modularity and ease of maintenance. Below is an example of a description file format:

```json
{
    "product": "ImageProcessor",
    "version": "2025.1",
    "include": [
        "./core.json",
        "./process.json"
    ]
}
```

#### Section JSON File Format

The section JSON file is structured to define the core modules and nodes of the project. Each node represents a specific operation or task within the module. Below is a breakdown of how to declare each section in the section JSON file:

- `module`: Specifies the name of the module.
- `nodes`: An array of node objects, each representing a distinct operation.

Each node object includes the following properties:

- `name`: The name of the node.
- `description`: A brief description of the node's functionality.
- `icon`: The filename of the icon representing the node.
- `parameters`: An array of parameter objects, each defining an input or output parameter for the node.

Each parameter object includes:

- `name`: The name of the parameter.
- `io`: Specifies whether the parameter is an input (`in`) or output (`out`).
- `type`: The data type of the parameter (e.g., `string`, `object`).
- `default`: The default value for the parameter (if applicable).
- `file`: A Boolean indicating if the parameter is a file path.
- `title`: The title for the file dialog (if applicable).
- `filter`: The file filter for the file dialog (if applicable).

- `template`: The filename of the Python template script associated with the node.

Example structure:

```json
{
  "module": "Core",
  "nodes": [
    {
      "name": "Open Image",
      "description": "Open image for editing.",
      "icon": "open-image.ico",
      "parameters": [
        {
          "name": "Path",
          "io": "in",
          "type": "string",
          "default": "../../../dat/im/res/landscape.jpg",
          "file": true,
          "title": "Open Image",
          "filter": "Image Files (*.jpg;*.jpeg;*.png;*.bmp;*.gif);;All Files (*.*)"
        },
        {
          "name": "Image",
          "io": "out",
          "type": "object"
        }
      ],
      "template": "open-image.py"
    },
    {
      "name": "Save Image",
      "description": "Save image to file.",
      "icon": "save-image.ico",
      "parameters": [
        {
          "name": "Image",
          "io": "in",
          "type": "object"
        },
        {
          "name": "Path",
          "io": "in",
          "type": "string",
          "default": "../../../dat/im/res/result.jpg",
          "file": true,
          "title": "Save Image As",
          "filter": "Image Files (*.jpg;*.jpeg;*.png;*.bmp;*.gif);;All Files (*.*)"
        }
      ],
      "template": "save-image.py"
    }
  ]
}
```

### Load Data

#### Install Packages

```shell
cd dat/my_workflow
python -m venv .env
source .env/bin/activate
python -m pip install -r requirements.txt
```

#### Start Modeling

- Run `visParaflow`
- Click `Open Pallet` and select `my_workflow/main.json` to load your workflow pallet definitions
- Click node and place it in the scene viewer area
- Drag to connect node parameters
- Once it's down, click `Generate Script` to generate script file, or `Run Workflow` to run the script directly
- Click `Save Workflow` to save the workflow
