

var c = document.getElementById("myCanvas");
c.addEventListener('click', () => {
    console.log('canvas clicked');
 });
var ctx = c.getContext("2d");
//Baterry Matrix and Generator
drawRec(ctx , 600,250,2,7,70,60,10,40);
//Countdown Ligth Flash
drawCir(ctx,100,400,1,3,20,30,5);
drawRec(ctx ,300,100,1,1,100,50,0,0);
drawRec(ctx ,310,110,1,1,80,30,0,0);
drawCir(ctx,200,130,1,1,30,0,0);
drawCir(ctx,200,130,1,1,20,0,0);
writeText(ctx,)

function drawRec(cnotext,xPos,yPos,row,col,width,height,xdis,ydis){
    for(var i = 0; i < row; i++){
        for(var j = 0; j < col; j++){
            verticallDistance = (ydis + height) * (i);
            horizontalDistance = (xdis + width) * (j);
            cnotext.beginPath();
            cnotext.rect(xPos + horizontalDistance, yPos + verticallDistance, width, height);
            cnotext.stroke();
        }
    }
}

function drawCir(cnotext,xPos,yPos,row,col,radius,xdis,ydis){
    for(var i = 0; i < row; i++){
        for(var j = 0; j < col; j++){
            verticallDistance = (ydis + radius) * (i);
            horizontalDistance = (xdis + radius) * (j);
            cnotext.beginPath();
            ctx.beginPath();
            ctx.arc(xPos + horizontalDistance, yPos +verticallDistance, radius, 0, 2 * Math.PI);
            ctx.stroke();
        }
    }
}
function writeText(cnotext,xPos,yPos,text){
    cnotext.font = "30px Arial";
    cnotext.strokeText(text,xPos,yPos);
}

function isIntersect(point, circle) {
    return Math.sqrt((point.x-circle.x) ** 2 + (point.y - circle.y) ** 2) < circle.radius;
}