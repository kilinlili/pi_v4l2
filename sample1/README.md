# V4L2でラズパイカメラを動かす

## Document
http://v4l.videotechnology.com/dwg/v4l2.html#V4L2-CAPABILITY

## Point

・カメラ性能(デバイス情報)はv4l2_capabilityに格納(V4L2API)

・xioctlでioctlをラップするサンプルコードがあるが，とりあえずioctlで実装

・例えばVIDIOC_REQBUFS-->VIDIOC_QUERYBUFSという順でioctlが叩かれる



- フォーマット-v4l2_format(VIDIOC_S_FMT)
- バッファ確保-v4l2_requestbuffers(VIDIOC_REQBUFS)

ここで処理方法が2パターン:どちらもエンキュー処理だと思うが...
A.ループを使用-v4l2_buffer(VIDIOC_QUERYBUF, VIDIOC_QBUF)
B.単純に抜き出すだけ-v4l2_buffer(VIDIOC_QUERYBUF)
-バッファクエリ





