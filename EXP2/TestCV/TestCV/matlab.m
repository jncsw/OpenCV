clear;   %��������˫���Բ�ֵ��ʵ��ͼ������    
I=imread('exp2.jpg'); %����ԭͼ��ֻ�轫�˴����ļ�����Ҫ�任��ͼƬ���� %ͼ������    
R = I(:,:,1);  
G = I(:,:,2);  
B = I(:,:,3);  
[rows,cols,coms]=size(I); %ͼ��ĸߺͿ�  
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
imshow(I),title('ԭͼ');  
figure;  
imshow(Out),title('�޸�ͼ');  