Đây là chương trình nhận diện khuôn mặt xử lý hình ảnh qua http request. Người dùng sẽ gửi lên 1 tấm ảnh, chương trình 
sẽ so sánh với bộ ảnh trong database rồi trả về kết quả.

Thuật toán:
	- Khi chạy FaceComServer.exe chương trình sẽ tự động load ảnh mẫu trong folder "FacesData\\Hinh_3x4_out"
	để huấn luyện. Trong folder chứa ảnh đó mỗi người sẽ được sắp xếp vào từng folder.
	- Chương trình sẽ luôn chạy để lắng nghe http request với method là POST, body là hình ảnh, header có 1 key là cropped,
	value là "true" hoặc "false" (lưu ý value là string). 
	- Gửi request tới địa chỉ IP của máy, port 8080
	- Khi nhận được hình ảnh từ phía client, chương trình sẽ nhận diện hình ảnh và trả về 1 chuỗi json có 3 field:
		+ name: tên người, nếu không nhận diện được sẽ là ""
		+ confident: độ chính xác (càng thấp càng chính xác)
		+ error: mã lỗi (nếu có)

		
Cách tạo database hình ảnh:
	- Dùng script create_sample.bat để crop ảnh gốc thành ảnh khuôn mặt, tham số là -createsample, tiếp theo là thư mục input và output
	- Tạo các folder rỗng với tên người
	- Sau khi crop, move ảnh mỗi người vào folder có tên của người đó
	- Folder tổng chứa tất cả folder được khai báo trong file FaceComp_config.json, hiện tại là "FacesData\\Hinh_3x4_out"
	=> xong
		
		
Lưu ý:
	- Chương trình cần có kết nối internet để hoạt động
	- Các tham số của chương trình trong file FaceComp_config.json
	- confident value mặc định là 80, nếu ảnh có confident lớn hơn confident value thì không trả về kết quả
	- Sử dụng PostMan để test http request: https://www.getpostman.com/apps
	- Tham số cropped ý nghĩa là ảnh đã được crop khuôn mặt, chương trình sẽ không detect mà chỉ nhận diện. Nghĩa là
	nếu gửi ảnh gốc thì không cần tham số này
	- Mỗi người có thể có 1 hoặc nhiều ảnh
	
	
Mọi chi tiết liên hệ vohungvi@vohungvi.com