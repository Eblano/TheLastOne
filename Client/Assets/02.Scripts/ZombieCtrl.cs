﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// NavMeshAgent 컴포넌트를 사용하기위해 추가해야하는 네임스페이스
using System;
using UnityEngine.AI;

public class ZombieCtrl : MonoBehaviour
{
    // 좀비의 상태 정보가 있는 Enumerable 변수 선언
    public enum ZombieState { idle, walk, attack, die };
    // 몬스터의 현재상태 정보를 저장할 Enum 변수
    public ZombieState zombieState = ZombieState.walk;

    // 속도 향상을 위해 각종 컴포넌트를 변수에 할당
    private Transform zombieTr;
    private Transform playerTr;
    private PlayerCtrl playerCtrl;
    private NavMeshAgent nvAgent;
    private Animator animator;

    // 추적 사정거리
    public float traceDist = 200.0f;
    // 공격 사정거리
    public float attackDist = 4.0f;

    // 좀비 공격력
    public int damage = 50;

    // 좀비의 사망 여부
    private bool isDie = false;

    // 혈흔 효과 프리팹
    public GameObject bloodEffect;

    // 좀비 체력 변수
    public int hp = 100;

    // 좀비 Nav 켜기
    private bool stopPos = false;    // NetworkCtrl Pos 고정을 멈추게 한다.
    public int zombieNum = -1;
    public int targetPlayer = -1;
    public int animator_value = 0;

    // Use this for initialization
    void Start()
    {
        // 좀비의 Transform 할당
        zombieTr = this.gameObject.GetComponent<Transform>();
        // 추적 대상인 Player의 Transform 할당
        playerTr = GameObject.FindWithTag("Player").GetComponent<Transform>();
        playerCtrl = GameObject.FindWithTag("Player").GetComponent<PlayerCtrl>();
        // NavMeshAgent 컴포넌트 할당
        nvAgent = this.gameObject.GetComponent<NavMeshAgent>();
        // Animator 컴포넌트 할당
        animator = this.gameObject.GetComponent<Animator>();

        // 추적 대상의 위치를 설정하면 바로 추적 시작
        nvAgent.destination = playerTr.position;

        // 일정한 간격으로 좀비의 행동 상태를 체크하는 코루틴 함수 실행
        StartCoroutine(this.CheckZombieState());

        // 좀비의 상태에 따라 동작하는 루틴을 실행하는 코루틴 함수 실행
        StartCoroutine(this.ZombieAction());

        // 좀비의 Nav를 키기 위한 코루틴
        StartCoroutine(this.CheckZombieNav());
    }

    //void OnEnable()
    //{
    //    PlayerCtrl.OnPlayerDie += this.OnPlayerDie;
    //}

    //// 이벤트 발생 시 연결된 함수 해제
    //void OnDisable()
    //{
    //    PlayerCtrl.OnPlayerDie -= this.OnPlayerDie;
    //}

    public IEnumerator CheckZombieNav()
    {
        while (true)
        {
            if (zombieTr.position.x != 0 && zombieTr.position.y != 0 && zombieTr.position.z != 0)
            {
                nvAgent.enabled = false;
                nvAgent.enabled = true;
                stopPos = true;
                break;
            }
            yield return new WaitForSeconds(0.2f);
        }
    }

    public IEnumerator CheckZombieState()
    {
        while (!isDie)
        {
            if ((targetPlayer == -1 || playerCtrl.Client_imei == targetPlayer) && targetPlayer != -2)
            {
                // 좀비의 타겟이 없을 경우.
                float dist = Vector3.Distance(playerTr.position, zombieTr.position);
                if (dist <= attackDist)
                {
                    // 공격거리 범위 이내로 들어왔는지 확인{
                    zombieState = ZombieState.attack;
                    playerCtrl.send_ZombieData(zombieTr.position, zombieTr.eulerAngles, zombieNum, zombieState, 0);
                }
                else if (dist <= traceDist)
                {
                    // 플레이어 추격위치에 도달할 경우
                    zombieState = ZombieState.walk;
                    playerCtrl.send_ZombieData(zombieTr.position, zombieTr.eulerAngles, zombieNum, zombieState, 0);
                }
                else if (dist > traceDist)
                {
                    // 플레이어 추격위치에 멀어질 경우
                    zombieState = ZombieState.idle;
                    playerCtrl.send_ZombieData(zombieTr.position, zombieTr.eulerAngles, zombieNum, zombieState, -2);
                    // -2를 한 이유는 플레이어가 좀비 보내줄때 -1은 예외처리를 하기 떄문에
                }
            }
            else if (playerCtrl.Client_imei != targetPlayer && targetPlayer != -1)
            {
                // 좀비의 Target과 자신의 IMEI가 다른경우 애니메이션 동기화만 해준다.
                switch (animator_value)
                {
                    case 0:
                        zombieState = ZombieState.idle;
                        break;
                    case 1:
                        zombieState = ZombieState.walk;
                        break;
                    case 2:
                        zombieState = ZombieState.attack;
                        break;
                    case 3:
                        zombieState = ZombieState.die;
                        animator.SetTrigger("IsDie");
                        StopAllCoroutines();
                        break;
                }
            }

            yield return new WaitForSeconds(0.2f);
        }


    }

    public IEnumerator ZombieAction()
    {
        while (!isDie)
        {
            switch (zombieState)
            {
                case ZombieState.idle:
                    nvAgent.isStopped = true;
                    animator.SetBool("IsTrace", false);
                    animator.SetBool("IsAttack", false);
                    break;

                // 추적 상태
                case ZombieState.walk:
                    // 추적 대상의 위치를 넘겨줌
                    nvAgent.destination = playerTr.position;
                    // 추적을 재시작
                    nvAgent.isStopped = false;
                    animator.SetBool("IsTrace", true);
                    animator.SetBool("IsAttack", false);
                    break;

                case ZombieState.attack:
                    nvAgent.isStopped = true;
                    animator.SetBool("IsTrace", false);
                    animator.SetBool("IsAttack", true);
                    break;
            }
            yield return null;
        }
    }

    void OnTriggerEnter(Collider coll)
    {
        //Debug.Log(coll.tag);
        // 충돌한 게임오브젝트의 태그값 비교
        if (coll.gameObject.tag == "BULLET")
        {
            CreateBloodEffect(coll.transform.position);

            // 맞은 총알의 Damage를 추출해 Zombie HP 차감
            hp -= coll.gameObject.GetComponent<BulletCtrl>().damage;
            playerCtrl.send_ZombieHP(zombieNum, hp);

            if (hp <= 0)
                ZombieDie();

            // Bullet 삭제
            Destroy(coll.gameObject);
        }
    }

    public void ZombieDie()
    {
        // 모든 코루틴 종료
        zombieState = ZombieState.die;
        nvAgent.isStopped = true;
        animator.SetTrigger("IsDie");
        isDie = true;
        hp = 0;
        playerCtrl.send_ZombieData(zombieTr.position, zombieTr.eulerAngles, zombieNum, zombieState, 0);
        StopAllCoroutines();

        gameObject.GetComponentInChildren<SphereCollider>().enabled = false;
        gameObject.GetComponent<CapsuleCollider>().enabled = false;

    }

    void CreateBloodEffect(Vector3 pos)
    {
        // 혈흔 효과 생성
        GameObject blood1 = (GameObject)Instantiate(bloodEffect, pos, Quaternion.identity);
        Destroy(blood1, 1.0f);
    }

    // 플레이어가 사망했을 때 실행되는 함수
    void OnPlayerDie()
    {
        // 좀비의 상태를 체크하는 코루틴 함수를 모두 정지시킴
        StopAllCoroutines();
        animator.SetBool("IsTrace", false);
        animator.SetBool("IsAttack", false);
    }

    public float DistanceToPoint(Vector3 a, Vector3 b)
    {
        // 캐릭터 간의 거리 구하기.
        return (float)Math.Sqrt(Math.Pow(a.x - b.x, 2) + Math.Pow(a.z - b.z, 2));
    }

    public void MovePos(Vector3 pos)
    {
        float moveSpeed = 10.0f;

        if (DistanceToPoint(zombieTr.position, pos) >= 20)
        {
            // 20이상 거리 차이가 날경우 움직여 주는것이 아닌 바로 동기화를 시켜 버린다.
            zombieTr.position = pos;
        }
        else
        {
            zombieTr.position = Vector3.MoveTowards(zombieTr.position, pos, Time.deltaTime * moveSpeed);
        }
    }
}
