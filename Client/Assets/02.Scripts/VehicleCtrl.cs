﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VehicleCtrl : PlayerVehicleCtrl
// PlayerVehicleCtrl을 쓴 이유는 Player를 GetComponent로 가져오는 방법 보다는 상속을 받는것이
// 오버헤드를 줄이고 실시간으로 데이터 값을 확인할 수 있어서.
{
    // 최고 속도
    public float maxTorque = 4000.0f;

    // 차량 체력
    public int vehicleHp = 200;

    public int vehicleInitHp;

    // 휠 콜라이더와 휠 메쉬 할당
    public WheelCollider[] wheelColliders = new WheelCollider[4];
    public Transform[] tireMeshs = new Transform[4];

    // 리지드바디 변수
    private Rigidbody m_rigidbody;


    private Transform vehicle_tr;

    public bool carDestroy = false;

    // 폭팔된 차량 및 이펙트
    public GameObject expCar;
    public GameObject expEffect;


    // 차량 자체의 브레이크를 위함
    public bool vehicleStop = false;


    void Awake()
    {
        // 생명 초기값 설정
        vehicleInitHp = vehicleHp;

        // 차량의 무게중심을 중심으로 맞춘다.
        vehicle_tr = GetComponent<Transform>();
        m_rigidbody = GetComponent<Rigidbody>();

        // 무게 중심 위치 잡아준다.
        m_rigidbody.centerOfMass = new Vector3(0, 0, 0);
    }

    void FixedUpdate()
    {
        // 차량 탑승 후에도 좌우 화면 전환이 가능하게 수정
        if (GetTheCar == true)
        {
            float h = Input.GetAxis("Horizontal");
            float v = Input.GetAxis("Vertical");
            VehicleCtrl_tr.transform.Rotate(Vector3.up * Time.deltaTime * 50.0f * Input.GetAxis("Mouse X"));

            // 바퀴 회전의 랜더링을 위함
            UpdateMeshsPositions();

            // 차량 브레이크 변수가 true일 경우 자체 브레이크 작동
            if (vehicleStop == true)
            {
                for (int i = 0; i < 4; i++)
                {
                    wheelColliders[i].brakeTorque = 0.1f;
                }
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    wheelColliders[i].brakeTorque = 0.0f;
                }
            }
        }
    }

    void UpdateMeshsPositions()
    {
        for (int i = 0; i < 4; i++)
        {
            Quaternion quat;
            Vector3 pos;
            wheelColliders[i].GetWorldPose(out pos, out quat);

            tireMeshs[i].position = pos;
            tireMeshs[i].rotation = quat;
        }
    }

    void OnTriggerEnter(Collider coll)
    {
        // 충돌한 게임오브젝트의 태그값 비교
        if (coll.gameObject.tag == "BULLET")
        {
            // 맞은 총알의 Damage를 추출해 Player HP 차감
            vehicleHp -= coll.gameObject.GetComponent<BulletCtrl>().damage;
            if (GetTheCar == true)
            {
                vehicleHpBar.fillAmount = (float)vehicleHp / (float)vehicleInitHp;
            }
            //    player.imgHpBar.fillAmount = (float)player.hp / (float)player.initHp;


            if (vehicleHp <= 0 && carDestroy == false)
            {
                ExpCar();
                carDestroy = true;
                gameObject.SetActive(false);

                // 미래를 위한 예비 코드
                //if(player.GetTheCar == true)
                //{
                //    player.hp = 0;
                //    player.imgHpBar.fillAmount = (float)player.hp / (float)player.initHp;
                //    player.PlayerDie();
                //}
            }
        }
    }

    void ExpCar()
    {
        // 폭팔 효과 파티클 생성
        Instantiate(expEffect, vehicle_tr.position, Quaternion.identity);
        Instantiate(expCar, vehicle_tr.position, Quaternion.identity);
    }
}
