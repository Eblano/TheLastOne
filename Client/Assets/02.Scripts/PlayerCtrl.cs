﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(AudioSource))]

public class PlayerCtrl : MonoBehaviour
{
    // 캐릭터의 상태 정보가 있는 Enumerable 변수 선언
    public enum PlayerState { run, fire };

    // 캐릭터의 현재 상태 정보를 저장할 Enum 변수
    public PlayerState playerState = PlayerState.run;

    private float h = 0.0f;
    private float v = 0.0f;

    // 접근해야 하는 컴포넌트는 반드시 변수에 할당한 후 사용
    private Transform tr;
    private Animator animator;

    // 캐릭터 이동 속도 변수
    public float moveSpeed = 23.0f;
    // 캐릭터 회전 속도 변수
    public float rotSpeed = 100.0f;

    // 총알 프리팹
    public GameObject bullet;
    // 총알 발사 좌표
    public Transform firePos;
    // 총알 발사 사운드
    public AudioClip fireSfx;

    // AudioSource 컴포넌트를 저장할 변수
    private AudioSource source = null;

    // MuzzleFlash의 MeshRenderer 컴포넌트 연결 변수
    public MeshRenderer muzzleFlash1;
    public MeshRenderer muzzleFlash2;

    // 마우스 고정 관련한 변수
    public bool lockMouse = false;

    // 카메라 뷰 전환을 체크하기 위한 변수
    public bool sensorCheck = false;

    void Start()
    {
        // 스크립트 처음에 Transform 컴포넌트 할당
        tr = GetComponent<Transform>();

        // Animator 컴포넌트 할당
        animator = this.transform.GetChild(0).GetComponent<Animator>();

        // AudioSource 컴포넌트를 추출한 후 변수에 할당
        source = GetComponent<AudioSource>();

        // 최초의 MuzzleFlash MeshRenderer를 비활성화
        muzzleFlash1.enabled = false;
        muzzleFlash2.enabled = false;

        animator.SetBool("IsTrace", false);

        // 처음 시작시 마우스를 잠궈버린다.
        lockMouse = true;
        Cursor.lockState = CursorLockMode.Locked;//마우스 커서 고정
        Cursor.visible = false;//마우스 커서 보이기

    }

    void Update()
    {
        h = Input.GetAxis("Horizontal");
        v = Input.GetAxis("Vertical");

        //Debug.Log("H=" + h.ToString());
        //Debug.Log("V=" + v.ToString());

        // 전후좌우 이동 방향 벡터 계산
        Vector3 moveDir = (Vector3.forward * v) + (Vector3.right * h);

        // Translate(이동 방향 * 속도 * 변위값 * Time.deltaTime, 기준 좌표)
        tr.Translate(moveDir.normalized * Time.deltaTime * moveSpeed, Space.Self);

        // Vector3.up 축을 기준으로 rotSpeed만큼의 속도로 회전
        tr.Rotate(Vector3.up * Time.deltaTime * rotSpeed * Input.GetAxis("Mouse X"));

        if (Input.GetMouseButtonDown(0))
        {
            Fire();
            animator.SetBool("IsTrace", true);

        }

        if (Input.GetMouseButtonDown(1))
        {
            Debug.Log("마우스 언락");
            Cursor.lockState = CursorLockMode.None;//마우스 커서 고정 해제
            Cursor.visible = true;//마우스 커서 보이기
            lockMouse = false;
        }

    }


    void Fire()
    {
        // 동적으로 총알을 생성하는 함수
        CreateBullet();

        // 사운드 발생 함수
        source.PlayOneShot(fireSfx, 0.9f);

        // 잠시 기다리는 루틴을 위해 코루틴 함수로 호출
        StartCoroutine(this.ShowMuzzleFlash());
    }

    void CreateBullet()
    {
        // Bullet 프리팹을 동적으로 생성
        Instantiate(bullet, firePos.position, firePos.rotation);
    }

    // MuzzleFlash 활성 / 비활성화를 짧은 시간 동안 반복
    IEnumerator ShowMuzzleFlash()
    {
        // MuzzleFlash 스케일을 불규칙하게 변경
        float scale = Random.Range(0.05f, 0.2f);
        muzzleFlash1.transform.localScale = Vector3.one * scale;
        muzzleFlash2.transform.localScale = Vector3.one * scale;

        // 활성화해서 보이게 함
        muzzleFlash1.enabled = true;
        muzzleFlash2.enabled = true;

        // 불규칙적인 시간 동안 Delay한 다음MeshRenderer를 비활성화
        yield return new WaitForSeconds(Random.Range(0.05f, 0.03f));

        // 비활성화해서 보이지 않게 함
        muzzleFlash1.enabled = false;
        muzzleFlash2.enabled = false;
    }

    void OnTriggerEnter(Collider coll)
    {

        // 충돌한 Collider가 Camchange의 CAMCHANGE(Tag값)이면 카메라 전환 
        if (coll.gameObject.tag == "CAMCHANGE")
        {

            FollowCam followCam = GameObject.Find("Main Camera").GetComponent<FollowCam>();
            if (sensorCheck == false)
            {
                followCam.change = true;
                Debug.Log("체크 인");
                followCam.height = 2.5f;
                followCam.dist = 7.0f;
                sensorCheck = true;
            }
            else if (sensorCheck == true)
            {
                followCam.change = false;
                Debug.Log("체크 아웃");
                followCam.height = 45.0f;
                followCam.dist = 20.0f;
                sensorCheck = false;
            }
        }
    }
}

