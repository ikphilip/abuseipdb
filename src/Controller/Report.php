<?php

namespace Drupal\abuseipdb\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\abuseipdb\Controller\Request;
use Drupal\ban\BanIpManager;

/**
 * Handles the reporting of IP addresses
 */
class Report extends ControllerBase {

  protected $api_key;
  protected $ban_manager;
  protected $categories;
  protected $cidr;
  protected $comment = '';
  protected $days = 30;
  protected $ip;
  protected $request;
  protected $response = NULL;
  protected $verbose = FALSE;

  /**
   * Initialize values which will be passed to a report request
   *
   * @param array $options
   * @return void
   */
  public function __construct(array $options = []) {
    foreach($options as $key => $value) {
      $this->{$key} = $value;
    }

    // Set the REST API request controller
    $this->request = new Request();

    // Set API key
    $api_key = \Drupal::config('abuseipdb.settings')->get('abuseipdb.api_key');
    $this->setApiKey($api_key);

    // Remove categories which are unused
    if ($this->categories) {
      $this->removeEmptyCategories($this->categories);
    }
  }

  public function ban() {
    $this->checkBanManager();
    $this->ban_manager->banIp($this->ip);
  }

  public function isBanned() {
    $this->checkBanManager();
    return $this->ban_manager->isBanned($this->ip);
  }

  protected function checkBanManager() {
    if (!$this->ban_manager) {
      $this->setBanManager();
    }
  }

  protected function setBanManager() {
    $connection = \Drupal::service('database');
    $this->ban_manager = new BanIpManager($connection);
  }

  public function check() {
    $this->response = $this->request->check($this->api_key, $this->ip, $this->days);
  }

  public function report() {
    $this->response = $this->request->report($this->api_key, $this->ip, $this->categories, $this->comment);
  }

  public function isAbusive() {
    $body = $this->getResponseBody();
    $body_array = json_decode($body);
    if (!empty($body_array)) {
      return TRUE;
    }

    return FALSE;
  }

  public function getApiKey() {
    return $this->api_key;
  }

  public function setApiKey(string $api_key) {
    $this->api_key = $api_key;
  }

  public function getCategories() {
    return $this->categories;
  }

  public function removeEmptyCategories(array &$categories = []) {
    rsort($categories);
    while (count($categories) > 0 && $categories[count($categories) - 1] == 0) {
      array_pop($categories);
    }
  }

  public function setCategories(array $categories) {
    $this->removeEmptyCategories($categories);
    $this->categories = $categories;
  }

  public function getComment() {
    return $this->comment;
  }

  public function setComment(string $comment) {
    $this->comment = $comment;
  }

  public function getDays() {
    return $this->days;
  }

  public function setDays(int $days) {
    $this->days = $days;
  }

  public function getIp() {
    return $this->ip;
  }

  public function setIp(string $ip) {
    $this->ip = $ip;
  }

  public function getResponseBody() {
    return ($this->response) ? $this->response->getBody() : FALSE;
  }

  public function getResponseStatusCode() {
    return ($this->response) ? $this->response->getStatusCode() : FALSE;
  }

  public function getVerbose() {
    return $this->verbose;
  }

  public function setVerbose(bool $verbose) {
    $this->verbose = $verbose;
  }
}
