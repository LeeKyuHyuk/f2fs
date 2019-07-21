On-disk Layout
--------------

F2FS는 전체 볼륨을 여러 개의 Segment로 나눕니다. 각각의 Segment는 2MB 크기로 고정됩니다. Section은 연속적인 Segment로 구성되며 Zone은 Section의 집합으로 구성됩니다. 기본적으로 Section 및 Zone의 크기는 동일한 Segment 크기로 동일하게 설정되지만 사용자는 `mkfs`로 크기를 쉽게 수정할 수 있습니다.

F2FS는 전체 볼륨을 6개의 영역으로 분할하며, Super Block을 제외한 모든 영역은 아래에 설명 된대로 여러 Segment로 구성됩니다.

```
                                            align with the zone size <-|
                 |-> align with the segment size
     _________________________________________________________________________
    |            |            |   Segment   |    Node     |   Segment  |      |
    | Superblock | Checkpoint |    Info.    |   Address   |   Summary  | Main |
    |    (SB)    |   (CP)     | Table (SIT) | Table (NAT) | Area (SSA) |      |
    |____________|_____2______|______N______|______N______|______N_____|__N___|
                                                                       .      .
                                                             .                .
                                                 .                            .
                                    ._________________________________________.
                                    |_Segment_|_..._|_Segment_|_..._|_Segment_|
                                    .           .
                                    ._________._________
                                    |_section_|__...__|_
                                    .            .
                                    .________.
                                    |__zone__|
```

- **Superblock (SB)** : 파티션의 시작 부분에 위치하며 파일 시스템 충돌을 피하기 위해 두 개의 복사본이 있습니다. 기본 파티션 정보와 F2FS의 일부 Default Parameters를 포함합니다.

- **Checkpoint (CP)** : 여기에는 파일 시스템 정보와 유효한 NAT/SIT의 Bitmap, 부모가 없는 inode 목록 및 현재 활성 Segment의 요약 항목이 들어 있습니다.

- **Segment Information Table (SIT)** : 유효한 블록 수 및 모든 블록의 유효성을 나타내는 Bitmap과 같은 Segment 정보를 포함합니다.

- **Node Address Table (NAT)** : Main Area에 저장된 모든 Node Block에 대한 블록 주소 테이블로 구성됩니다.

- **Segment Summary Area (SSA)** : Main Area에 저장된 모든 데이터 및 Node Block의 소유자 정보를 포함하는 요약 항목을 포함합니다.

- **Main Area** : 색인(indices)을 포함한 파일 및 디렉토리 데이터가 들어 있습니다.

파일 시스템과 플래시 기반 스토리지 간의 잘못된 정렬을 피하기 위해 F2FS는 CP(Checkpoint)의 시작 블록 주소를 Segment 크기에 맞춥니다. 또한 SSA(Segment Summary Area) 영역의 일부 Segment를 예약하여 Main 영역의 시작 블록 주소를 Zone 크기로 정렬합니다.

File System Metadata Structure
------------------------------

F2FS는 checkpoint 체계를 사용하여 파일 시스템 일관성을 유지합니다. 마운트 할 때 F2FS는 먼저 CP 영역을 스캔하여 마지막 유효한 checkpoint 데이터를 찾습니다. 검색 시간을 줄이기 위해 F2FS는 두 개의 CP 복사본만 사용합니다. 그 중 하나는 항상 Shadow Copy Mechanism이라고하는 마지막 유효한 데이터를 나타냅니다. CP외에도 NAT와 SIT도 Shadow Copy Mechanism을 사용합니다.

파일 시스템 일관성을 위해 각 CP는 NAT 및 SIT 복사본의 유효한 지점을 가리킵니다. (아래 그림 참조)

```
  +--------+----------+---------+
  |   CP   |    SIT   |   NAT   |
  +--------+----------+---------+
  .         .          .          .
  .            .              .              .
  .               .                 .                 .
  +-------+-------+--------+--------+--------+--------+
  | CP #0 | CP #1 | SIT #0 | SIT #1 | NAT #0 | NAT #1 |
  +-------+-------+--------+--------+--------+--------+
     |             ^                          ^
     |             |                          |
     `----------------------------------------'
```
