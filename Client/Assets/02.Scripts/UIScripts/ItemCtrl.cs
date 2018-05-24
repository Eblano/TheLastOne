﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ItemCtrl : InventoryCtrl
{

    public enum ItemType { Ammunition762, Ammunition556, Ammunition9, FirstAid, ProofVest } // 탄약, 구급상자

    public ItemType type;           // 아이템의 타입.
    public Sprite DefaultImg;   // 기본 이미지.
    public int MaxCount;        // 겹칠수 있는 최대 숫자.
    public int itemCount = 0;

    public int getItemCount() { return itemCount; } 
    public void setItemCount(int value) { itemCount += value; }
    public void ResetItemCount(int value) { itemCount = value; }

    void AddItem()
    {
        // 아이템 획득에 실패할 경우.
        if (!AddItem(this))
            Debug.Log("아이템이 가득 찼습니다.");
        else // 아이템 획득에 성공할 경우.
            gameObject.SetActive(false); // 아이템을 비활성화 시켜준다.
    }

    // 충돌체크
    void OnTriggerEnter(Collider _col)
    {
        // 플레이어와 충돌하면.
        if (_col.gameObject.layer == 10)
            AddItem();
    }
}
