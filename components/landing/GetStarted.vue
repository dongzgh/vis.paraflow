<script setup>
import defineNodes from '@/assets/img/define-nodes.png';
import loadNodes from '@/assets/img/load-nodes.png';
import modelWorkflow from '@/assets/img/model-workflow.png';
import generateScript from '@/assets/img/generate-script.png';

const steps = [
  {
    title: 'Define Nodes',
    picture: defineNodes,
  },
  {
    title: 'Load Nodes',
    picture: loadNodes,
  },
  {
    title: 'Model Workflow',
    picture: modelWorkflow,
  },
  {
    title: 'Generate Scipt',
    picture: generateScript,
  },
];
</script>

<template>
  <div id="GetStarted" class="place-items-center mt-16 md:mt-0">
    <h2 class="text-4xl lg:text-5xl font-bold lg:tracking-tight">
      How to Get Started with visParaflow
    </h2>
    <p class="text-lg mt-4 text-slate-600">
      Follow the simple steps below to get started working on your next amazing
      project.
    </p>
  </div>

  <div class="grid sm:grid-cols-2 md:grid-cols-4 place-items-top mt-16 mb-16">
    <div v-for="step of steps" class="flex gap-4 items-start">
      <div class="mt-1 bg-black rounded-full p-2 w-8 h-8 shrink-0">
        <Icon class="text-white" name="bx:bxs-chevron-right" />
      </div>
      <div>
        <h3 class="font-semibold text-lg">{{ step.title }}</h3>
        <div class="md:order-1 hidden md:block">
          <img
            class="transition-transform transform hover:scale-110"
            :src="step.picture"
            :alt="step.title"
            loading="eager"
            format="avif"
            width="300"
            height="300"
          />
        </div>
      </div>
    </div>
  </div>
</template>
