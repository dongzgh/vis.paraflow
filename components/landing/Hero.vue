<template>
  <main class="grid lg:grid-cols-2 place-items-center pt-16 pb-8 md:pt-8">
    <div class="p-12 md:order-1 hidden md:block">
      <img
        class="transition-transform transform hover:scale-110"
        src="~/assets/img/hero.png"
        alt="Starship starts the engine"
        loading="eager"
        format="avif"
        width="800"
        height="800"
      />
    </div>

    <div>
      <h1 class="text-5xl lg:text-6xl xl:text-7xl font-bold lg:tracking-tight">
        Model Workflows and Generate Scripts Made Easy
      </h1>
      <p class="text-lg mt-4 text-slate-600 max-w-xl">
        visParaflow is a tool that helps you model workflows and generate scripts
        for your projects. It bridges the gap between GUI based modeling and scripting.
        It runs on Windows, macOS and Linux.
      </p>
      <div class="mt-6 flex flex-col sm:flex-row gap-3">
        <LandingLink
          href="https://github.com/dongzgh/vis.paraflow"
          target="_blank"
          rel="noopener"
          >Github Repo</LandingLink
        >
      </div>
    </div>
  </main>
</template>
