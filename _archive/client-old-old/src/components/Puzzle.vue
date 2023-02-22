<template>
  <div>
    <div v-if="keyName === 'power_control'">
      <puzzle1 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle1>
    </div>
    <div v-if="keyName === 'status_board'">
      <puzzle2 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle2>
    </div>
    <div v-if="keyName === 'datamatic'">
      <puzzle3 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle3>
    </div>
    <div v-if="keyName === 'safeomatic'">
      <puzzle4 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle4>
    </div>
    <div v-if="keyName === 'rcp'">
      <puzzle5 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle5>
    </div>
    <div v-if="keyName === 'life_support'">
      <puzzle6 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle6>
    </div>
    <div v-if="keyName === 'docked_ship'">
      <puzzle7 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle7>
    </div>
    <div v-if="keyName === 'prep_status'">
      <puzzle8 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle8>
    </div>
    <div v-if="keyName === 'lasergrid'">
      <puzzle9 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle9>
    </div>
    <div v-if="keyName === 'keypad'">
      <puzzle10 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle10>
    </div>
    <div v-if="keyName === 'laserbar'">
      <puzzle11 
        v-bind:id="id"
        v-bind:name="name"
        v-bind:key-name="keyName"
        v-bind:state="state"
        v-bind:state-names="stateNames"
        ></puzzle11>
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

export default {
  props: ['id', 'name', 'keyName', 'state', 'stateNames'],
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
    puzzle11: Puzzle11
  }
};
</script>