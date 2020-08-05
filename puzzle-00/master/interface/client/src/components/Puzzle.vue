<template>
  <div>
    <!-- {{ id }}
    {{ keyName }} -->

    <div v-if="keyName === 'power_control'">
      <puzzle1 
        v-bind:id="id"
        ></puzzle1>
    </div>
    <div v-if="keyName === 'status_board'">
      <puzzle2 
        v-bind:id="id"
        ></puzzle2>
    </div>
    <div v-if="keyName === 'datamatic'">
      <puzzle3 
        v-bind:id="id"
        ></puzzle3>
    </div>
    <div v-if="keyName === 'safeomatic'">
      <puzzle4 
        v-bind:id="id"
        ></puzzle4>
    </div>
    <div v-if="keyName === 'rcp'">
      <puzzle5 
        v-bind:id="id"
        ></puzzle5>
    </div>
    <div v-if="keyName === 'life_support'">
      <puzzle6 
        v-bind:id="id"
        ></puzzle6>
    </div>
    <div v-if="keyName === 'docked_ship'">
      <puzzle7 
        v-bind:id="id"
        ></puzzle7>
    </div>
    <div v-if="keyName === 'prep_status'">
      <puzzle8 
        v-bind:id="id"
        ></puzzle8>
    </div>
    <div v-if="keyName === 'lasergrid'">
      <puzzle9 
        v-bind:id="id"
        ></puzzle9>
    </div>
    <div v-if="keyName === 'keypad'">
      <puzzle10 
        v-bind:id="id"
        ></puzzle10>
    </div>
    <div v-if="keyName === 'laserbar'">
      <puzzle11 
        v-bind:id="id"
        ></puzzle11>
    </div>
    <div v-if="keyName === 'puzzle_12'">
      <puzzle12 
        v-bind:id="id"
        ></puzzle12>
    </div>
    <div v-if="keyName === 'puzzle_13'">
      <puzzle13></puzzle13>
    </div>
  </div>
</template>

<script>
import Puzzle1 from './Puzzle1.vue';
import Puzzle2 from './Puzzle2.vue';
import Puzzle3 from './Puzzle3.vue';
import Puzzle4 from './Puzzle4.vue';
import Puzzle5 from './Puzzle5.vue';
import Puzzle6 from './Puzzle6.vue';
import Puzzle7 from './Puzzle7.vue';
import Puzzle8 from './Puzzle8.vue';
import Puzzle9 from './Puzzle9.vue';
import Puzzle10 from './Puzzle10.vue';
import Puzzle11 from './Puzzle11.vue';
import Puzzle12 from './Puzzle12.vue';
import Puzzle13 from './Puzzle13.vue';

export default {
  props: ['id', 'keyName'],
  components: {
    puzzle1: Puzzle1,
    puzzle2: Puzzle2,
    puzzle3: Puzzle3,
    puzzle4: Puzzle4,
    puzzle5: Puzzle5,
    puzzle6: Puzzle6,
    puzzle7: Puzzle7,
    puzzle8: Puzzle8,
    puzzle9: Puzzle9,
    puzzle10: Puzzle10,
    puzzle11: Puzzle11,
    puzzle12: Puzzle12,
    puzzle13: Puzzle13
  }
};
</script>