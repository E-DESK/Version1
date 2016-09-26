Các thuật toán trong E-DESK:
#define Ánh_sáng_đèn_bàn_nhiệt_độ_độ_ẩm:
	+ If lux > LUX_OK -> Do nothing.
	+ If lux < LUX_OK -> Turn on the light.
		Or:
	+ LIGHT = lux*(LUX_OK)/100.
	+ PWM the LIGHT
	**Cảnh báo nhiệt độ quá nóng, độ ẩm quá cao.

#define Counter:
	+Thời gian ngồi làm việc:
		$$$$$ Tính time:
		if(giờ < 23h)
		{
			time += time_liên_tục
		}

		*IF 		( time >8h / day) 	=>"~~You work to much, Let go out and save the worlk.~~"
		*ELSE_IF 	( 8 > time > 4) 	=>"<<To day is a happy day>>"
		*ELSE_IF	( time < 4 )		=>"!!!Come on, you know what this line mean!!!"
		
		$$$$ Tính time liên tục:
				If(đang ngồi)
					Time_liên_tục+= counter_minutes
				else if ( không ngồi )
				{
					đợi ( 5p ) 
					nếu ( không ngồi ) => time_liên_tục =0;
					nếu ( ngồi lại ) => Time_liên_tục tiếp tục; 
				}

		IF( time_liên_tục > 2h) => "Go out and Jump Jump Jump."
		IF( time_liên_tục > 4h)	=> "Can you feel your legs?"
		IF( time_liên_tục > 8h)	=> "Shit, Are you sleep?"

	+Struct History{Time,Time_liên_tục,Time_trong tháng}
		
#define Calender:/*WHAT THE HELL IS THUẬT TOÁN GÌ ĐỂ NHẮC VIỆC*/
	+ 10 Calender: dùng vector hoặc linklist.
		*Tạo linklist
		*Thêm linklist
		*Duyệt linklist
		*Xóa link list
	+ Sắp xếp theo ngày tháng: "so sánh năm,tháng,ngày,giờ,phút" -> đánh số ưu tiên.
		*Tìm kiếm trong linklist.
	+Thông báo khi: "năm = năm_; tháng= tháng_.....; phút = phút_ - 15" (thông báo trước 15p);
	+Sent bluetooth.

#define Bảo_mật:
	+if(nhận "UNLOCK")
	{
		IF("MỞ TỦ") => Do nothing
		ELSE("KHÔNG MỞ")
		{
			Đếm 5p => "LOCK"
		}	
	}
	else
	{//Không nhận
		IF("MỞ TỦ") => Camera => SDCard => Sent thingspeak.
		ELSE("KHÔNG MỞ") => Do nothing
	}

#define Đánh_giá_độ_cân_bằng_cuộc_sống:
{"Quá tệ","Tạm ổn","Ổn", "Tuyệt", "Quá tuyệt"}

	*Nếu "làm việc nhiều" 	and "Liên tục" 			=> "Quá tệ"
	*Nếu "làm việc nhiều" 	and "Không liên tục" 	=> "Tạm ổn"
	*Nếu "làm việc vừa" 	and "Liên tục" 			=> "Ổn"
	*Nếu "làm việc vừa" 	and "Không liên tục" 	=> "Quá tuyệt"
	*Nếu "làm việc ít" 		and "Liên tục" 			=> "Tuyệt"
	*Nếu "làm việc ít" 		and "Không liên tục" 	=> "Quá tệ"