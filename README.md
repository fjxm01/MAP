**1. Point 기반 MAP 함수 사용법**
  -

1. 입력 형태
  - calculator.MAPrun(cloud, group Assigments, group Index)
  - PCflt: 그룹으로 지정된 점들
  - PGa: 각 그룹 ID에 포함된 점들
  - g: 그룹 ID
2. 옵션 설정
  - calculator.MAPrun(cloud, group Assigments, group Index, debug)
  - MAP의 동작이 정확하게 되고 있는지 시각화 하는 디버깅 옵션이며 기본적으로 false로 되어있기에 True로 바꾸면 디버깅이 활성화 된다.
3. 함수 사용
  1) MAP 실행
  - calculator.MAPrun(result.PCflt, result.PGa, g);
  
  2) MAP 시각화 

  - result 값은 다음과 같이 저장한다.
     
      firstMAP[g] = pointRes.firstMAP;

      lastMAP[g] = pointRes.lastMAP;

      avgMAP[g] = pointRes.meanMAP;
     
      minMAP[g] = pointRes.minMAP;
     
      maxMAP[g] = pointRes.maxMAP;
     
  - PC_viz에서 MAP 시각화를 불러와 위에 결과 값을 넣어준다. 
<img width="962" height="572" alt="image" src="https://github.com/user-attachments/assets/0782123e-c3bb-4739-bf07-fa69b986914e" />

**Point 기반 MAP 설명**
  -

정의)

MAP란 Multiple Average Probability의 약자로 다중 확률 평균을 말한다. 이는 하나의 그룹 특징을 추출하는 방법으로 밀도 기반의 특징을 추출하는 방법이다. 
Point 기반의 MAP는 오직 점들의 구성으로만 가지고 특징을 추출하는 방법이다. 

알고리즘 순서도)
<img width="903" height="504" alt="image" src="https://github.com/user-attachments/assets/c27ce6d7-2c0b-44ed-a9db-bc8851b0ef5f" />
  
**2. Normal 기반 MAP 함수 사용법**
  -

1. 입력 형태
  - calculator.MAPrunNormals(cloud, group Assigments, group Index)
  - PCflt: 그룹으로 지정된 점들
  - PGa: 각 그룹 ID에 포함된 점들
  - g: 그룹 ID
2. 옵션 설정
  - calculator.MAPrunNormals(cloud, group Assigments, group Index, debug)
  - MAP의 동작이 정확하게 되고 있는지 시각화 하는 디버깅 옵션이며 기본적으로 false로 되어있기에 True로 바꾸면 디버깅이 활성화 된다.
3. 함수 사용
  1) MAP 실행
  - calculator.MAPrunNormals(result.PCflt, result.PGa, g);
  
  2) MAP 시각화 

  - result 값은 다음과 같이 저장한다.
     
      firstMAP[g] = pointRes.firstMAP;

      lastMAP[g] = pointRes.lastMAP;

      avgMAP[g] = pointRes.meanMAP;
     
      minMAP[g] = pointRes.minMAP;
     
      maxMAP[g] = pointRes.maxMAP;
     
  - PC_viz에서 MAP 시각화를 불러와 위에 결과 값을 넣어준다. 
<img width="962" height="572" alt="image" src="https://github.com/user-attachments/assets/d6a41d38-33e5-4c97-af25-8a8bc2d5aa66" />

**Normal 기반 MAP 설명**
  -

정의)

Point 기반 MAP의 방식과 같으나 점만을 가지고 특징을 구하는 것이 아닌 Normal Vector를 추가하여 더 정확한 특징을 추출하는 방법이다. 

알고리즘 순서도) 
<img width="901" height="513" alt="image" src="https://github.com/user-attachments/assets/1e78024b-0627-4a1f-a7ea-7e5507923353" />
