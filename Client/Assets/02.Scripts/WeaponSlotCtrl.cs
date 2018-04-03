﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class WeaponSlotCtrl : MonoBehaviour
{
    public Stack<WeaponCtrl> weaponSlot;       // 슬롯을 스택으로 만든다
    public Text text;       // 아이템에 개수를 표현해줄 텍스트
    public Sprite DefaultImg; // 슬롯에 있는 아이템을 다 사용할 경우 아무것도 없는 이미지를 넣어줄 필요가 있다

    public Image WeaponImg;
    private bool isSlot;     // 현재 슬롯이 비어있는지?

    public int slotNumber;

    public WeaponCtrl WeaponReturn() { return weaponSlot.Peek(); } // 슬롯에 존재하는 아이템이 뭔지 반환

    //public bool ItemMax(ItemCtrl item) { return ItemReturn().MaxCount > weaponSlot.Count; } // 겹칠수 있는 한계치를 넘으면

    public bool isSlots() { return isSlot; } // 슬롯이 현재 비어있는지? 에 대한 플래그 반환

    public void SetSlots(bool value) { this.isSlot = value; }

    public PlayerCtrl player;

    void Start()
    {
        // 스택 메모리 할당.
        weaponSlot = new Stack<WeaponCtrl>();

        // 맨 처음엔 슬롯이 비어있다.
        isSlot = false;

        WeaponImg = transform.GetChild(0).GetComponent<Image>();

        player = GameObject.FindWithTag("Player").GetComponent<PlayerCtrl>();

        
    }

    public void AddWeapon(WeaponCtrl weapon, bool same, WeaponSlotCtrl sameSlot)
    {
        // bool same = true 일경우 아이템 같은게 들어왔으니 찾아서 넣어라.
        // 스택에 아이템 추가.
        //if (same == true)
        //{
        //    //Debug.Log(sameSlot.ItemReturn().type.ToString());
        //    sameSlot.WeaponReturn().setItemCount(30); // 이미 존재 하면 해당 갯수를 증가한다.
        //}
        //else
        //{

        //    if (weapon.type.ToString() == "Ammunition762" || weapon.type.ToString() == "Ammunition556")
        //        weapon.setItemCount(29);// 총알의 경우 기본이 1개 이므로 29개를 더해서 30개로 맞춰준다.
        //    weaponSlot.Push(weapon);
        //}

        // 무기 추가
        weaponSlot.Push(weapon);

        // 정보 갱신
        UpdateInfo(true, weapon.DefaultImg);
    }

    // 무기 버리는 함수
    public void WeaponThrow()
    {
        // 슬롯이 비어있으면 함수를 종료.
        if (!isSlot)
            return;

        // 슬롯에 아이템이 1개인 경우.
        // 아이템이 1개일 때 사용하게 되면 0개가 된다.
        if (weaponSlot.Peek().getItemCount() == 1)
        {
            // 혹시 모를 오류를 방지하기 위해 slot리스트를 Clear해준다
            weaponSlot.Clear();
            // 아이템 사용으로 인해 아이템 개수를 표현하는 텍스트가 달라졌으므로 업데이트 시켜준다.
            UpdateInfo(false, DefaultImg);
            return;
        }

        // 정보 갱신
        UpdateInfo(isSlot, WeaponImg.sprite);
    }

    // 슬롯에 대한 각종 정보 업데이트.
    public void UpdateInfo(bool value, Sprite sprite)
    {
        //// 인벤토리에 변동사항이 생겼으므로 저장
        this.isSlot = value;
        transform.GetChild(0).GetComponent<Image>().sprite = sprite;

        if (weaponSlot.Count >= 1)
            text.text = " [ "+slotNumber + " ] " + weaponSlot.Peek().type.ToString();
        else
            text.text = " [ " + slotNumber + " ] ";

    }
}