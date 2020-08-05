<template>
  <div>
    <form action id="frm">
      <svg width="1100" height="400" fill="black">
        <!-- Background -->
        <rect
          x="0"
          y="0"
          width="1100"
          height="400"
          fill="LightGray"
          stroke="black"
          stroke-width="1"
        />
        <!-- title Laser -->
        <text x="690" y="80" style="fill: none; stroke: #000000;  font-size: 42px;">Laser</text>

        <!-- Keypad  -->
        <text x="230" y="120" style="fill: none; stroke: #000000;  font-size: 30px;">Status</text>
        <rect
          x="180"
          y="90"
          width="340"
          height="300"
          fill-opacity="0"
          fill
          stroke="black"
          stroke-width="1"
        />
        <foreignObject x="230" y="150" width="200" height="150">
          <input name="txtLaserGrid" type="text" value="No Data" />
        </foreignObject>

        <!-- Connection Status -->
        <text x="750" y="300" style="fill: none; stroke: #000000;  font-size: 30px;">Connection:</text>
        <foreignObject x="940" y="280" width="200" height="150">
          <input name="txtConnection" type="text" style="width:150px;" value="No Connection" />
        </foreignObject>

        <!-- Puzzle Status -->
        <text x="750" y="360" style="fill: none; stroke: #000000;  font-size: 30px;">Puzzle Status:</text>
        <foreignObject x="940" y="340" width="200" height="150">
          <input name="txtStatus" type="text" style="width:150px;" value="No Data " />
        </foreignObject>
      </svg>
    </form>
  </div>
</template>

<script>
export default {
  props: []
};
</script>