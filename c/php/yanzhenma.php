<?php
$num=4; 
//验证码个数
$distance=30;
$width=$num*$distance;
$height=30;
$im=imagecreatetruecolor($width,$height);
$color=imagecolorallocate($im,1,11,211);
$color_pixel=imagecolorallocate($im,235,23,29);
$bg=imagecolorallocate($im,210,210,210);
$font = 'C:/Windows/Fonts/arial.ttf';
$yanzhengma=getcode($num,0);
$size=22;
//字的大小
//
$angle=rand(-30,30); 
$text=$yanzhengma;

//1生成随机验证码
function getcode($m=4,$type=0){
	$str="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	$t = array(9,35,strlen($str)-1); $c=""; 
	for($i=0; $i < $m;$i ) { 
		$c.=$str[rand(0,$t[$type])] ; 
	} 
	return $c;
}
// 2开始绘画 
imagefill($im,0,0,$bg);
//填充背景色 
//生成干扰点 
for($i=0;$i < 300;$i ) { 
	$c=imagecolorallocate($im,rand(0,255),rand(0,255),rand(0,255)); 
	imagesetpixel($im,rand(0,$width),rand(0,$height),$c); 
}
 //生成随机线段
 for($i=0;$i < 10;$i ) { 
	$c=imagecolorallocate($im,rand(0,255),rand(0,255),rand(0,255)); 
	imageline($im,rand(0,$width),rand(0,$height),rand(0,$width),rand(0,$height),$c); 
}
 //将随机验证码写入图片上 
for($i=0;$i < $num;$i ) { 
	imagettftext($im, $size, rand(-30,30), 5 , $distance*$i, 27, $color, $font, $text[$i]); 
}
 header("Content-Type:image/png"); 
//显示图像 
imagepng($im); 
//销毁图像 
imagedestroy($im);

?>
