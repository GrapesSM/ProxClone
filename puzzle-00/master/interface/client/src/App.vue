<template>
  <div id="app">
    <navigation />
    <router-view />
  </div>
</template>

<script>
import Navigation from "components/navigation";
import { USER_REQUEST } from "actions/user";

export default {
  components: {
    Navigation
  },
  name: "app",
  created: function() {
    if (this.$store.getters.isAunthenticated) {
      this.$store.dispatch(USER_REQUEST);
    }
  }
};
</script>

<style>
#app {
  margin-top: 60px
}
</style>
