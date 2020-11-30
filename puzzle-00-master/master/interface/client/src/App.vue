<template>
  <div id="app">
    <navigation />
    <router-view />
  </div>
</template>

<script>
import Navigation from "./components/Navigation.vue";

export default {
  components: {
    Navigation
  },
  name: "app"
};
</script>

<style>
#app {
  margin-top: 60px
}
</style>
