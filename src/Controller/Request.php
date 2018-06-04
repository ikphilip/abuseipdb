<?php

namespace Drupal\abuseipdb\Controller;

use Drupal\Core\Controller\ControllerBase;
use GuzzleHttp\Client;

/**
 * Handles sending and receiving requests to the AbuseIPDB API
 */
class Request extends ControllerBase {

  private $client;

  function __construct() {
    $this->client = new Client([
      'base_uri' => 'https://www.abuseipdb.com/'
    ]);
  }

  /**
   * Delivers a report to AbuseIPDB API
   *
   * @param string $api_key
   * @param string $ip
   * @param array $categories
   * @param string $comment
   * @return object Guzzle response
   */
  public function report(string $api_key, string $ip, array $categories, string $comment = '') {
    $query = [];
    $query['key'] = $api_key;
    $query['category'] = implode(',', $categories);
    $query['comment'] = $comment;
    $query['ip'] = $ip;

    $res = $this->client->request('POST', 'report/json', ['query' => $query, 'http_errors' => false]);

    return $res;
  }

  /**
   * Query the AbuseIPDB API for IP reports
   *
   * @param string $api_key
   * @param string $ip
   * @param integer $days
   * @return object Guzzle response
   */
  public function check(string $api_key, string $ip, int $days = NULL) {
    $query = [];
    $query['api_key'] = $api_key;

    $res = $this->client->request('GET', 'check/' . $ip . '/json', ['query' => $query, 'http_errors' => false]);

    return $res;
  }
}