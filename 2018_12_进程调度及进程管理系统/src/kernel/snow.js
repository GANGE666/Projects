var snow={
	x:0,y:0,alpha:0.5,radius:3,velocity:{x:0,y:5
	},
	draw:function(c)
	{
		c.fillStyle='rgba(255,255,255,'+this.alpha+')';
		c.beginPath();
		c.arc(this.x,this.y,this.radius,0,Math.PI*2,true);
		c.fill();
	},
	update:function(){
		this.x+=this.velocity.x
		this.y+=this.velocity.y;
	},
	randomize:function(zone){
		var s=this.getLocation(zone);
		this.x=s.x;this.y=s.y;
		this.alpha=this.randomRange(0.3,1);
		this.radius=this.randomRange(1,5);
		this.velocity={
			x:this.randomRange(-0.35,0.35),
			y:this.randomRange(0.75,1.5)}
		},
		action:function(){
			this.velocity.x+=(Math.random()-0.5)*0.1;
		},
		randomRange:function(low,high){
			return(Math.random()*(high-low))+low;
		},
		getLocation:function(z){
			var p={};p.x=z.x+Math.random()*z.width;
			p.y=z.y+Math.random()*z.height;return p;
		}
	};