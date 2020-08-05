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
        <!-- Power Switch -->
        <rect
          id="power"
          x="40"
          y="20"
          width="30"
          height="30"
          fill="white"
          stroke="black"
          stroke-width="1"
        />
        <text x="90" y="40" style="fill: none; stroke: #000000;  font-size: 30px;">POWER</text>
        <!-- title Docked Ship -->
        <text x="800" y="80" style="fill: none; stroke: #000000;  font-size: 42px;">DATAMATIC</text>
        <!-- Code  -->
        <text x="300" y="100" style="fill: none; stroke: #000000;  font-size: 30px;">CODE</text>
        <foreignObject x="400" y="75" width="200" height="150">
          <input name="txtCode" type="text" value="No Data" />
        </foreignObject>

        <!-- Picture Show Mode -->
        <text
          x="70"
          y="150"
          style="fill: none; stroke: #000000;  font-size: 30px;"
        >Picture Show Series:</text>
        <foreignObject x="390" y="130" width="200" height="150">
          <input name="txtSeries" type="text" value="No Data" />
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
    <br />
    <table style="width:100%">
      <tr>
        <th>Series</th>
        <th>Picture</th>
        <th>Picture Number</th>
      </tr>
      <tr>
        <td>Series1</td>
        <td>
          <img src="/img/puzzle-04-images/page0.jpg" alt="Smiley face" height="90" />
        </td>
        <td>0</td>
      </tr>
      <tr>
        <td rowspan="2">Series2</td>
        <td>
          <img src="/img/puzzle-04-images/page1.jpg" alt="Smiley face" height="90" />
        </td>
        <td>1</td>
      </tr>
      <tr>
        <td>
          <img src="/img/puzzle-04-images/page2.jpg" alt="Smiley face" height="90" />
        </td>
        <td>2</td>
      </tr>
      <tr>
        <td rowspan="3">Series3</td>
        <td>
          <img src="/img/puzzle-04-images/page3.jpg" alt="Smiley face" height="90" />
        </td>
        <td>3</td>
      </tr>
      <tr>
        <td>
          <img src="/img/puzzle-04-images/page4.jpg" alt="Smiley face" height="90" />
        </td>
        <td>4</td>
      </tr>
      <tr>
        <td>
          <img src="/img/puzzle-04-images/page5.jpg" alt="Smiley face" height="90" />
        </td>
        <td>5</td>
      </tr>
    </table>
  </div>
</template>

<script>
export default {
  props: []
};
</script>