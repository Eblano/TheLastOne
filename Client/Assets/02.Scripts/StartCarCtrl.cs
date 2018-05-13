﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using TheLastOne.GameClass;

public class StartCarCtrl : MonoBehaviour
{

    // 자동차 이동 속도
    public float speed = 3000.0f;

    public GameObject player;
    private PlayerCtrl Player_Script;

    private bool startSet = false;
    private bool dontStart = true;
    private int waitTime = 0;
    Game_ProtocolClass recv_protocol = new Game_ProtocolClass();

    IEnumerator waitTimeOnline()
    {
        // 온라인으로 접속을 하였지만 너무 오랜 시간 기다릴 경우를 대비하여 n초후 출발하게 한다.
        do
        {
            if (SingletonCtrl.Instance_S.startCarStatus == false)
            {
                // 모든 플레이어가 인게임 상태일 경우
                waitTime++;
            }
            if (waitTime >= 5)
            {
                // 5초이상 기다려도 모든 팀원이 준비가 안되어 있을경우 그냥 출발한다.
                GetComponent<Rigidbody>().AddForce(transform.forward * speed);
                StopCoroutine(waitTimeOnline());
                dontStart = false;
            }
            yield return new WaitForSeconds(1.0f);
        } while (dontStart);
        //yield return null;
    }


    IEnumerator waitStartCar()
    {
        do
        {
            if (SingletonCtrl.Instance_S.startCarStatus == true)
            {
                // 모든 플레이어가 인게임 상태일 경우
                GetComponent<Rigidbody>().AddForce(transform.forward * speed);
                StopCoroutine(waitStartCar());
                dontStart = false;
            }
            yield return null;
        } while (dontStart);
        //yield return null;
    }


    // Use this for initialization
    void Start()
    {
        player.GetComponent<PlayerCtrl>().enabled = false;
        Player_Script = GameObject.FindWithTag("Player").GetComponent<PlayerCtrl>();

        // 플레이어는 인게임 상태로 들어왔다.
        SingletonCtrl.Instance_S.PlayerStatus = recv_protocol.playGameStatus;

        // 게임 시작후 차량 하차 시 인벤토리 창을 끈다.
        // 쿨타임 스크립트 할당
        Player_Script.cooltimeCtrl = GameObject.Find("PanelCoolTime").GetComponent<CoolTimeCtrl>();
        Player_Script.inventory.SetActive(false);
        Player_Script.cooltime.SetActive(false);
        Player_Script.VehicleUI.SetActive(false);


        if (SingletonCtrl.Instance_S.PlayerSocket.Connected == false)
        {
            // 서버와 연결이 되어있지 않을경우 바로 출발한다.
            GetComponent<Rigidbody>().AddForce(transform.forward * speed);
            dontStart = false;
        }
        else
        {
            // 서버와 연결이 되어있을 경우 출발 대기를 한다.
            StartCoroutine(waitStartCar());
            StartCoroutine(waitTimeOnline());
        }
    }

    void Update()
    {
        if (dontStart == false)
        {
            // 서버에서 시작차량을 출발하라고 한 뒤 부터 F를 누를 수 있다.
            if (startSet != true)
            {
                player.transform.position = new Vector3(this.transform.position.x, 30.0f, this.transform.position.z);

                //------------------------------------------------------------------------------
                // 차량 이동 중 에도 지도를 볼 수 있다.
                if (Input.GetKeyDown(KeyCode.CapsLock))
                {
                    if (Player_Script.realView == false)
                    {
                        Player_Script.realMap.SetActive(true);
                        Player_Script.realView = true;
                    }
                    else if (Player_Script.realView == true)
                    {
                        Player_Script.realMap.SetActive(false);
                        Player_Script.realView = false;
                    }
                }

                if (Player_Script.realView == true)
                {
                    Player_Script.playerPositionImage.localPosition = new Vector3(-gameObject.transform.position.z * 0.5f, gameObject.transform.position.x * 0.5f);
                    Player_Script.playerPositionImage.eulerAngles = new Vector3(0, 0, player.transform.eulerAngles.y );
                }
                //------------------------------------------------------------------------------
            }
            // F키 입력 시
            if (Input.GetKeyDown(KeyCode.F) && startSet == false)
            {
                // 플레이어에 가해진 힘을 0으로 만든다. - > 차량 하차
                //player.GetComponent<Rigidbody>().velocity = Vector3.zero;

                // 플레이어 스크립트 사용 (이동 때문)
                player.GetComponent<PlayerCtrl>().enabled = true;

                // 카메라 전환
                FollowCam followCam = GameObject.Find("Main Camera").GetComponent<FollowCam>();
                followCam.getOff = true;
                followCam.height = 35.0f;
                followCam.dist = 25.0f;

                // 차량에 하차 할때 차량의 위치로 플레이어를 이동시킨다.
                player.transform.position = new Vector3(this.transform.position.x, 30.0f, this.transform.position.z);
                // 차량 하차 후 true로 F키 입력 시 재하차 불가능하게 만듬
                startSet = true;
            }
        }
    }

    private void OnTriggerEnter(Collider coll)
    {
        //충돌한 Collider가 Camchange의 CAMCHANGE(Tag값)이면 카메라 전환
        if (coll.gameObject.tag == "AllGetOff" && startSet == false)
        {
            // 플레이어에 가해진 힘을 0으로 만든다. - > 차량 하차
            player.GetComponent<Rigidbody>().velocity = Vector3.zero;
            // 플레이어 스크립트 사용 (이동 때문)
            player.GetComponent<PlayerCtrl>().enabled = true;

            // 차량에 하차 할때 차량의 위치로 플레이어를 이동시킨다.
            player.transform.position = new Vector3(this.transform.position.x, 30.0f, this.transform.position.z);

            // 카메라 전환
            FollowCam followCam = GameObject.Find("Main Camera").GetComponent<FollowCam>();
            followCam.getOff = true;
            followCam.height = 35.0f;
            followCam.dist = 25.0f;

            // 차량 하차 후 true로 F키 입력 시 재하차 불가능하게 만듬
            startSet = true;

            //bug.Log("차량 하차 -> 게임 시작");
        }

        if (coll.gameObject.tag == "EndPoint")
        {
            Destroy(gameObject);
        }
    }
}
