<script setup>
const features = [
  {
    title: "GUI Based Modeling",
    description:
      "Node based visual modeling tool. Drag, drop and connect nodes to create your own workflows. Save and load to reuse.",
    icon: "bx:bxs-paint",
  },
  {
    title: "Open Nodes Definition",
    description:
      "Define your own nodes and their templates. Use them in your workflows. Share them with others.",
    icon: "bx:bxs-cube-alt",
  },
  {
    title: "On Demand Node Sets",
    description:
      "Predefined node sets made available for common tasks. Customize and extend them to suit your own needs.",
    icon: "bx:bxs-data",
  },
  {
    title: "Cross Platform Supported",
    description:
      "Runs on Windows, macOS and Linux. Same user interface and same data. No need to worry about compatibility issues.",
    icon: "bx:bxs-file-find",
  },
  {
    title: "Automatic Script Generation",
    description:
      "One-click script generation. Python scripts is natively supported, but can be extended to other languages.",
    icon: "bx:bxs-file-find",
  },
  {
    title: "Easy Parametric Study",
    description:
      "No need to struggle with searching and modifying parameter values in scripts. What you see is what you get.",
    icon: "bx:bxs-user",
  },
];
</script>

<template>
  <div id="Features" class="place-items-center mt-16 md:mt-0">
    <h2 class="text-4xl lg:text-5xl font-bold lg:tracking-tight">
      Why visParaflow for Workflow Modeling
    </h2>
    <p class="text-lg mt-4 text-slate-600">
      Feature rich and easy to use. visParaflow is the tool you need for your next workflow modeling and script generation project.
    </p>
  </div>

  <div class="grid sm:grid-cols-2 md:grid-cols-3 place-items-center mt-16 mb-16">
    <div v-for="item of features" class="flex gap-4 items-start">
      <div class="mt-1 bg-black rounded-full p-2 w-8 h-8 shrink-0">
        <Icon class="text-white" :name="item.icon" />
      </div>
      <div>
        <h3 class="font-semibold text-lg">{{ item.title }}</h3>
        <p class="text-slate-500 mt-2 leading-relaxed">
          {{ item.description }}
        </p>
      </div>
    </div>
  </div>
</template>
