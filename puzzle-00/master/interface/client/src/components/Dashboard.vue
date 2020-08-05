<template>
  <div>
    <header>
      <navigation></navigation>
    </header>
    <section>
      <b-container fluid>
        <b-row>
          <b-col cols="2">
            <div class="sidebar-sticky">
              <ul class="nav">
                <li class="nav-item">
                  <b-link class="nav-link" 
                    v-bind:class="{ disabled: current === -1 }"
                    v-on:click="() => { setCurrent(-1) }">
                    <span data-feather="home"></span>
                    Dashboard
                    <span class="sr-only">(current)</span>
                  </b-link>
                </li>
              </ul>

              <ul class="nav flex-column">
                <li class="nav-item" v-for="(puzzle, index) in puzzles" :key="puzzle.key_name">
                  <b-link class="nav-link"
                    v-bind:class="{ disabled: current === index }"
                    v-on:click.stop="() => { setCurrent(index) }">
                    <span class="oi oi-puzzle-piece"></span>
                    {{ puzzle.name }}
                  </b-link>
                </li>
              </ul>

              <h6
                class="sidebar-heading d-flex justify-content-between align-items-center px-3 mt-4 mb-1 text-muted"
              >
                <span>Information</span>
              </h6>
              <ul class="nav flex-column mb-2">
                <li class="nav-item">
                  <b-link class="nav-link">
                    <span data-feather="file-text"></span>
                    Reports
                  </b-link>
                </li>
                <li class="nav-item">
                  <b-link class="nav-link">
                    <span data-feather="file-text"></span>
                    About
                  </b-link>
                </li>
                <li class="nav-item">
                  <b-link class="nav-link">
                    <span data-feather="file-text"></span>
                    Reference
                  </b-link>
                </li>
              </ul>
            </div>
          </b-col>
          <b-col cols="10">
            <puzzle
              v-if="current > -1"
              v-bind:id="puzzles[current].id"
              v-bind:key-name="puzzles[current].key_name"
            ></puzzle>
          </b-col>
        </b-row>
      </b-container>
    </section>
  </div>
</template>

<script>
import axios from "axios";
import Navigation from "./Navigation.vue";
import Puzzle from "./Puzzle.vue";

export default {
  data() {
    return {
      puzzles: [],
      current: -1
    };
  },
  components: {
    navigation: Navigation,
    puzzle: Puzzle
  },
  methods: {
    getPuzzles() {
      const path = "http://localhost:5000/puzzles";
      axios
        .get(path)
        .then(res => {
          this.puzzles = res.data.puzzles;
        })
        .catch(error => {
          // eslint-disable-next-line
          console.error(error);
        });
    },
    setCurrent(index) {
      this.current = index
    }
  },
  created() {
    this.getPuzzles();
  }
};
</script>
