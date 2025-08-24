module adder (a , clk ,rst ,c);
 
 
************Parameters Declaration*********** 
 
parameter width = 4'b1111 ;

 
 
************Ports Declaration*********** 
 
input  clk ;
input  rst ;
input wire[width-1:0] a ;
output reg[width-1:0] c ;

 
 
************Implementation Code*********** 
 
always @(posedge clk,negedge rst) begin 

 
 
 
 
end 
 
 
 
 
 
always @(*) begin 

 
 
 
 
end 
 
 
 
 
 
endmodule