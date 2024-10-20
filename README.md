﻿# Tetris

## Index

- [개요](#개요)
- [시현 영상](#시현-영상)
- [요구 사항 분석](#요구-사항-분석)
- [설계](#설계)
- [구현](#구현)

---

## 개요

언리얼 엔진으로 테트리스를 만드는 프로젝트

- 개발 기간: 2024.05.28 ~ (진행 중)
- 개발 환경:
  - 1인 개발
  - OS: Windows 11
  - Engine: Unreal Engine 5.1.1
  - IDE: Visual Studio 2022
  - AI coding assistant: GitHub Copilot
  - Source Control: Git, GitHub

### 프로젝트 진행 방식

[2009 Tetris Design Guideline](./Documents/2009_Tetris_Design_Guideline.pdf)을 준수하되,  
처음부터 모든 걸 설계한 후에 구현하지 않고 하나씩 기능을 추가하면서 점진적으로 개발.

개인 프로젝트이지만 단위 기능 구현마다 [Pull Request](https://github.com/keunbum/Tetris/pulls?q=is%3Apr+is%3Aclosed)를 통해 진행 상황을 정리하고 간단한 코드 리뷰 진행.

### Best Practice

- [TETRIS 99](https://namu.wiki/w/TETRIS%2099)
- [Tetris Effect](https://namu.wiki/w/Tetris%20Effect)

### Coding Standard
[Unreal Engine Coding Standard](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine)를 준수한다.

---

## 시현 영상

- (유튜브 링크 첨부 예정)

---

## 요구 사항 분석

[2009 Tetris Design Guideline](./Documents/2009_Tetris_Design_Guideline.pdf)의 일부를 수정 및 요약하였다.

### 1. 도입
#### 1.1. 기본 용어

##### **Line Clear**
가로 행이 블록으로 완전히 채워지면 매트릭스에서 제거되는 상태. 클리어된 라인 위의 모든 조각(piece)은 아래로 이동하여 공간을 채운다.  

##### **Lock Down**
플레이 중인(매트릭스에 나와 있는) 테트로미노가 잠겨 플레이어가 이를 더 이상 조작할 수 없는 지점.  
일반적으로 테트로미노가 지면(매트릭스의 바닥이나 기존 블록)에 착지하고 나서 0.5초 후에 발생한다.  

##### **Matrix**
활성 게임 영역을 만드는 직사각형 셀 배열로, 일반적으로 너비 10열 x 20행이다.  

##### **Mino**
테트로미노를 이루는 단일 정사각형 모양의 구성 요소.  

##### **Skyline**
매트릭스 상단의 수평선. 테트로미노는 스카이 라인 바로 위에서 떨어지기 시작한다.

##### **Tetromino(테트로미노)**
측면을 따라 연결된 4개의 미노들로 구성된 기하학적 Tetris® 모양.  
각각 고유한 색상으로 표시되는 4개의 미노들을 사용하여 총 7개의 테트로미노를 만들 수 있다.  

![Tetriminos](https://static.wikia.nocookie.net/tetrisconcept/images/c/ca/Tetromino_image.png/revision/latest?cb=20090706171943)


### 2. 기본 게임 흐름

#### 2.1 인터페이스

![Tetris Interface](./Documents/Tetris_Interface.png)

##### **1. The Matrix**
게임 플레이가 발생하는 영역이다. 표준 매트릭스 치수는 높이 20셀 x 너비 10셀이다.

##### **2. Tetromino in Play**
매트릭스에 들어와 있는 테트로미노.  
플레이어는 이 테트로미노를 오른쪽 또는 왼쪽으로 이동하고, 시계 방향 또는 반시계 방향으로 회전하고, 하드 또는 소프트 드롭으로 조작할 수 있다.  
하드 드롭을 하면 테트로미노가 즉시 똑바로 떨어져서 착지한 첫 번째 표면에 고정된다(잠긴다).  
소프트 드롭은 버튼을 놓을 때까지 현재 낙하 속도보다 20배 빠르게 테트로미노를 떨어뜨린다.

##### **3. Next Queue**
넥스트 큐를 통해 플레이어는 생성되어 플레이에 투입될 다음 테트로미노를 볼 수 있다.

##### **4. Ghost Piece**
**Tetromino in Play**의 복사본이며 플레이어에게 플레이 테트로미노가 현재 위치에서 "떨어지면" 멈출 위치를 알려준다. 고스트 피스는 테트로미노 윤곽선 또는 테트로미노의 반투명한 "유령" 이미지로 나타날 수 있다.

##### **5. Game Information**
플레이 중인 게임과 관련된 정보가 화면에 표시된다.

- 경과시간
- 현재 레벨
- 지워진 줄 수 / 지워야 할 남은 줄 수

##### **6. Hold Queue**
홀드 큐를 통해 플레이어는 떨어지는 테트리미노를 고정(홀드)할 수 있다.
홀드 큐에 이미 있는 테트리미노가 있는 경우 꺼내온다.

