﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LobbyCostumCtrl : MonoBehaviour
{

    public GameObject costumUI;
    public GameObject storeUI;
    public GameObject playButton;
    public GameObject mapButton;
    public Scrollbar UI_Scrollbar;

    public bool costumOnOff;

    public LobbyStoreCtrl storeCtrl;


    // v 표시 체크 아이콘을 위함
    public GameObject[] checkIcon = new GameObject[18];
    public int[] checkIconNumber = new int[18];

    // 자물쇠 체크 아이콘을 위함
    public GameObject[] lockIcon = new GameObject[18];
    public int[] lockIconNumber = new int[18];


    public GameObject[] playerCostumeView = new GameObject[18];

    private void Awake()
    {
        costumUI.SetActive(false);
        costumOnOff = false;


        // 수 초기화
        for (int i = 0; i < 18; ++i)
        {
            checkIconNumber[i] = 0;
            lockIconNumber[i] = 0;
        }

        // Costum을 읽어왔으니 Awake시 바로 정착 및 활성화 시켜 준다.
        checkIcon[SingletonCtrl.Instance_S.WereCostumNumber].SetActive(true);
        checkIconNumber[SingletonCtrl.Instance_S.WereCostumNumber] = 1;
        WereCostumeButton(SingletonCtrl.Instance_S.WereCostumNumber);

    }

    private void Update()
    {
        if (Input.GetAxis("Mouse ScrollWheel") < 0)
        {
            UI_Scrollbar.value -= 0.2f;
        }
        if (Input.GetAxis("Mouse ScrollWheel") > 0)
        {
            UI_Scrollbar.value += 0.2f;
        }
    }

    public void CostumButton()
    {
        
        if (costumOnOff == false)
        {
            costumUI.SetActive(true);
            costumOnOff = true;

            storeUI.SetActive(false);
            storeCtrl.storeOnOff = false;
            
            for (int i=0;i<18; ++i)
            {
                if (SingletonCtrl.Instance_S.PlayerCostume[i] == 0)
                    // 코스튬 없을 경우 잠금, 있을 경우 해제
                {
                    lockIconNumber[i] = 1;
                    lockIcon[i].SetActive(true);
                }
                else
                {
                    lockIconNumber[i] = 0;
                    lockIcon[i].SetActive(false);
                }

            }
        }
        else if (costumUI == true)
        {
            costumUI.SetActive(false);
            costumOnOff = false;
        }
        //// 2번의 경우, ZombieMode로 싱글톤 NowModeNumber에게 2을 넣어준다.
        //SingletonCtrl.Instance_S.NowModeNumber = 2;
        //SingletonCtrl.Instance_S.NowMapNumber = 2;
        //Debug.Log("현재 Zombie Mode를 선택하셨습니다 = " + SingletonCtrl.Instance_S.NowModeNumber);
    }

    public void WereCostumeButton(int number)
    {
        // checkIconNumber이 0이면 비활성화
        // checkIconNumber이 1이면 활성화
        if (lockIconNumber[number] == 1)
        {
            return;
        }

        // Costum 을 서버에 등록 하기 위하여.
            SingletonCtrl.Instance_S.WereCostumNumber = number;

        for (int i = 0; i < 18; ++i)
        {
           
            if (i != number)
            {
                checkIconNumber[i] = 0;
                checkIcon[i].SetActive(false);
                
                playerCostumeView[i].SetActive(false);

            }
            else if (i == number)
            {
                checkIconNumber[i] = 1;
                checkIcon[i].SetActive(true);            

                playerCostumeView[i].SetActive(true);
            }
        }
    }
}
