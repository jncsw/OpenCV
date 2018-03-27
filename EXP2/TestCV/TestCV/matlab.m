clear;   %此题是用双线性插值法实现图像缩放    
I=imread('exp2.jpg'); %读入原图像，只需将此处的文件换成要变换的图片即可 %图像属性    
R = I(:,:,1);  
G = I(:,:,2);  
B = I(:,:,3);  
[rows,cols,coms]=size(I); %图像的高和宽  
Out = uint8(zeros(rows,cols,coms));  
RR = uint8(zeros(rows,cols));  
GG = uint8(zeros(rows,cols));  
BB = uint8(zeros(rows,cols));  
for x=1:cols-1  
    xc=(x-0.5*cols)/(0.5*cols);  
    for y=1:rows-1  
        yc=(y-0.5*rows)/(0.5*rows);  
        r=sqrt(xc*xc+yc*yc);  
        s=(1-r)*(1-r);  
        if(r>=1)  
            orix=xc;  
            oriy=yc;  
            a = double(uint16(orix*0.5*cols+0.5*cols));                      
            b = double(uint16(oriy*0.5*rows+0.5*rows));  
            RR(y,x)=R(b,a);  
            GG(y,x)=G(b,a);  
            BB(y,x)=B(b,a);  
        else  
            orix=cos(s)*xc-sin(s)*yc;  
            oriy=sin(s)*xc+cos(s)*yc;  
            a = double(uint16(orix*0.5*cols+0.5*cols));                      
            b = double(uint16(oriy*0.5*rows+0.5*rows));  
            RR(y,x)=R(b,a);  
            GG(y,x)=G(b,a);  
            BB(y,x)=B(b,a);  
        end  
    end  
end  
Out(:,:,1) = RR;  
Out(:,:,2) = GG;  
Out(:,:,3) = BB;  
imshow(I),title('原图');  
figure;  
imshow(Out),title('修改图');  