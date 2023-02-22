<template>
  <div class="container">
    <label>Response: {{ msg }}</label>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: 'Ping',
  data() {
    return {
      msg: '',
    };
  },
  methods: {
    getMessage() {
      const path = `http://${process.env.API_URL}:5000/ping`;
      axios.get(path)
        .then((res) => {
          this.msg = res.data;
        })
        .catch((error) => {
          this.msg = error;
          console.error(error);
        });
    },
  },
  created() {
    this.getMessage();
  },
};
</script>
